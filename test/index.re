open Reduice.Promise;

[@decco]
type config = {
    clientId: string,
    secret: string,
    code: string,
    redirectUri: string,
};

Js.log(Auth.getAuthUrl(~state="stayt", "cli", [|Auth.YouTube|], "http://www.com/", Auth.Code));

let { clientId, secret, code, redirectUri } =
    Config.get("test") |> config_decode |> Belt.Result.getExn;

Auth.getTokensFromCode(clientId, secret, code, redirectUri)
|> then_(({ Auth.access_token }) => {
    Js.log2("token", access_token);
    People.getMe(access_token, [| People.Names |]);
})
|> PromiseEx.map(({ People.resourceName }) => Js.log2("resourceName", resourceName))
|> catch((exn) => {
    Js.log2("exn", exn);
    resolve();
});
