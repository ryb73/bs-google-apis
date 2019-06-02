open Superagent;
open PromEx;

[@decco]
type tokens = {
    access_token: string,
    refresh_token: option(string),
    expires_in: int
};

type responseType = Code | Token;
type accessType = Online | Offline;
type scope =
    | Profile | Email | YouTube | YouTubeSSL | YouTubeReadOnly | YouTubePartner
    | YouTubePartnerChannelAudit | Contacts | ContactsReadOnly | PlusLogin
    | UserAddressesRead | UserBirthdayRead | UserEmailsRead | UserPhoneNumbersRead
    | UserInfoEmail | UserInfoProfile;

let getResponseTypeString = fun
    | Code => "code"
    | Token => "token";

let getAccessTypeString = fun
    | Online => "online"
    | Offline => "offline";

let getScopeStringSingle = fun
    | Profile => "profile" | Email => "email"
    | YouTube => "https://www.googleapis.com/auth/youtube"
    | YouTubeSSL => "https://www.googleapis.com/auth/youtube.force-ssl"
    | YouTubeReadOnly => "https://www.googleapis.com/auth/youtube.readonly"
    | YouTubePartner => "https://www.googleapis.com/auth/youtubepartner"
    | YouTubePartnerChannelAudit => "https://www.googleapis.com/auth/youtubepartner-channel-audit"
    | Contacts => "https://www.googleapis.com/auth/contacts"
    | ContactsReadOnly => "https://www.googleapis.com/auth/contacts.readonly"
    | PlusLogin => "https://www.googleapis.com/auth/plus.login"
    | UserAddressesRead => "https://www.googleapis.com/auth/user.addresses.read"
    | UserBirthdayRead => "https://www.googleapis.com/auth/user.birthday.read"
    | UserEmailsRead => "https://www.googleapis.com/auth/user.emails.read"
    | UserPhoneNumbersRead => "https://www.googleapis.com/auth/user.phonenumbers.read"
    | UserInfoEmail => "https://www.googleapis.com/auth/userinfo.email"
    | UserInfoProfile => "https://www.googleapis.com/auth/userinfo.profile";

let getScopeString = (scopes) =>
    scopes
    |> Js.Array.map(getScopeStringSingle)
    |> Js.Array.joinWith(" ");

let accessTokenApiCall = (reqData) =>
    post("https://www.googleapis.com/oauth2/v4/token")
    |> setHeader(ContentType(ApplicationXWwwFormUrlencoded))
    |> sendDict(reqData)
    |> end_
    |> map(({ body }) => body
        |> Belt.Option.getExn
        |> tokens_decode
    )
    |> map(Belt.Result.getExn);

let getTokensFromCode = (~accessType=?, clientId, secret, code,  redirectUri) => {
    let opts = [|
        ("client_id", clientId),
        ("client_secret", secret),
        ("grant_type", "authorization_code"),
        ("code", code),
        ("redirect_uri", redirectUri)
    |];

    Belt.Option.map(accessType, (at) =>
        Js.Array.push(("access_type", getAccessTypeString(at)), opts));

    opts
    |> Js.Dict.fromArray
    |> Js.Dict.map([@bs] ((s) => Js.Json.string(s)))
    |> accessTokenApiCall;
};

let generateJwt = (scope, email, privateKey) =>
    [|
        ("iss", Js.Json.string(email)),
        ("scope", getScopeString(scope) |> Js.Json.string),
        ("aud", Js.Json.string("https://www.googleapis.com/oauth2/v4/token")),
        ("exp", (Js.Date.now() /. 1000. +. (60.*.60.)) |> floor |> Js.Json.number), /* 60min * 60sec = 1hr */
        ("iat", (Js.Date.now() /. 1000.) |> floor |> Js.Json.number)
    |]
    |> Js.Dict.fromArray
    |> Js.Json.object_
    |> JsonWebToken.sign(~algorithm=JsonWebToken.RS256, _, privateKey);

let getTokensForServiceAccount = (scope, email, privateKey) =>
    // https://developers.google.com/identity/protocols/OAuth2ServiceAccount
    [|
        ("grant_type", "urn:ietf:params:oauth:grant-type:jwt-bearer"),
        ("assertion", generateJwt(scope, email, privateKey))
    |]
    |> Js.Dict.fromArray
    |> Js.Dict.map([@bs] ((s) => Js.Json.string(s)))
    |> accessTokenApiCall;

let refreshAccessToken = (clientId, secret, refreshToken) =>
    [|
        ("refresh_token", refreshToken),
        ("client_id", clientId),
        ("client_secret", secret),
        ("grant_type", "refresh_token")
    |]
    |> Js.Dict.fromArray
    |> Js.Dict.map([@bs] ((s) => Js.Json.string(s)))
    |> accessTokenApiCall;

type prompt = None | Consent | SelectAccount;
let prompt_encode = fun
    | None => "none"
    | Consent => "consent"
    | SelectAccount => "select_account";

let getAuthUrl =
(~state=?, ~accessType=?, ~prompt=?, clientId, scopes, redirectUri, responseType) => {
    // https://developers.google.com/identity/protocols/OAuth2UserAgent

    let opts = [|
        ("client_id", clientId),
        ("redirect_uri", redirectUri),
        ("scope", getScopeString(scopes)),
        ("response_type", getResponseTypeString(responseType)),
    |];

    Belt.Option.map(state, (s) => Js.Array.push(("state", s), opts));
    Belt.Option.map(accessType, (at) =>
        Js.Array.push(("access_type", getAccessTypeString(at)), opts));
    Belt.Option.map(prompt, (p) => Js.Array.push(("prompt", prompt_encode(p)), opts));

    let qs = opts
    |> Js.Dict.fromArray
    |> Js.Dict.map([@bs] (v) => Js.Json.string(v))
    |> Qs.stringify;
    "https://accounts.google.com/o/oauth2/v2/auth?" ++ qs;
};
