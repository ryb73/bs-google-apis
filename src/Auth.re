open Superagent;
open PromiseEx;

[@decco]
type tokens = {
    access_token: string,
    refresh_token: option(string),
    expires_in: int
};

let _getOption = fun
    | Some(v) => v
    | None => failwith("Expected Some, got None");

let _accessTokenApiCall = (reqData) =>
    post("https://www.googleapis.com/oauth2/v4/token")
    |> setHeader(ContentType(ApplicationXWwwFormUrlencoded))
    |> send(reqData)
    |> end_
    |> map(({ body }) => body
        |> _getOption
        |> tokens_decode
    )
    |> unwrapResult;

let getTokensFromCode = (clientId, secret, code,  redirectUri) => {
    Js.Dict.fromList
        ([
            ("client_id", clientId),
            ("client_secret", secret),
            ("grant_type", "authorization_code"),
            ("code", code),
            ("redirect_uri", redirectUri)
        ])
        |> (v) => { Js.log(v); v }
        |> Js.Dict.map([@bs] ((s) => Js.Json.string(s)))
        |> Js.Json.object_
        |> _accessTokenApiCall;
};

type scope =
    | Profile
    | Email
    | YouTube
    | YouTubeSSL
    | YouTubeReadOnly
    | YouTubePartner
    | YouTubePartnerChannelAudit
    | Contacts
    | ContactsReadOnly
    | PlusLogin
    | UserAddressesRead
    | UserBirthdayRead
    | UserEmailsRead
    | UserPhoneNumbersRead
    | UserInfoEmail
    | UserInfoProfile;

let _getScopeStringSingle = fun
    | Profile => "profile"
    | Email => "email"
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
    | UserInfoProfile => "https://www.googleapis.com/auth/userinfo.profile"

let _getScopeString = (scopes) =>
    scopes
    |> Js.Array.map(_getScopeStringSingle)
    |> Js.Array.joinWith(" ");

type responseType = Code | Token;
type accessType = Online | Offline;

let _getResponseTypeString = fun
    | Code => "code"
    | Token => "token";

let _getAccessTypeString = fun
    | Online => "online"
    | Offline => "offline";

let getAuthUrl = (~state=?, ~accessType=?, clientId, scopes, redirectUri, responseType) => {
    let opts = [|
        ("client_id", clientId),
        ("redirect_uri", redirectUri),
        ("scope", _getScopeString(scopes)),
        ("response_type", _getResponseTypeString(responseType)),
    |];

    Belt.Option.map(state, (s) => Js.Array.push(("state", s), opts));
    Belt.Option.map(accessType, (at) => Js.Array.push(("access_type", _getAccessTypeString(at)), opts));

    let qs = opts
    |> Js.Dict.fromArray
    |> Js.Dict.map([@bs] (v) => Js.Json.string(v))
    |> Qs.stringify;
    "https://accounts.google.com/o/oauth2/v2/auth?" ++ qs;
};