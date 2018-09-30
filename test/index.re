open Reduice.Promise;

[@decco]
type config = {
    clientId: string,
    secret: string,
    code: string,
};

let { clientId, secret, code } =
    Config.get("test") |> config_decode |> Belt.Result.getExn;

Auth.getTokensFromCode(clientId, secret, code)
|> then_(({ Auth.access_token }) => {
    Js.log2("token", access_token);
    People.getMe(access_token, [| People.Names |]);
})
|> PromiseEx.map(({ People.resourceName }) => Js.log2("resourceName", resourceName))
|> catch((exn) => {
    Js.log2("exn", exn);
    resolve();
});
