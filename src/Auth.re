open Superagent;
open PromiseEx;

[@decco]
type tokens = {
    access_token: string,
    refresh_token: string,
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

let getTokensFromCode = (clientId, secret, code) => {
    Js.Dict.fromList
        ([
            ("client_id", clientId),
            ("client_secret", secret),
            ("grant_type", "authorization_code"),
            ("code", code),
            ("redirect_uri", "postmessage") /* https://stackoverflow.com/questions/11485271/google-oauth-2-authorization-error-redirect-uri-mismatch */
        ])
        |> Js.Dict.map([@bs] ((s) => Js.Json.string(s)))
        |> Js.Json.object_
        |> _accessTokenApiCall;
};