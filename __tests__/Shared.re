open PromiseEx;

[@decco]
type people = {
    userId: string
};

[@decco]
type config = {
    clientId: string,
    secret: string,
    code: string,
    redirectUri: string,
    people: people,
};

let { clientId, secret, code, redirectUri, people } =
    Config.get("test") |> config_decode |> Belt.Result.getExn;

let accessToken =
    Auth.getTokensFromCode(clientId, secret, code, redirectUri)
    |> map(({ Auth.access_token }) => access_token);