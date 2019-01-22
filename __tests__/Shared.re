open PromEx;

[@decco]
type serviceAccount = {
    email: string,
    privateKey: string
};

[@decco]
type people = {
    userId: string
};

[@decco]
type config = {
    serviceAccount: serviceAccount,
    people: people,
};

let { people, serviceAccount: { email, privateKey } } =
    Config.get("test") |> config_decode |> Belt.Result.getExn;

let accessToken =
    Auth.getTokensForServiceAccount([| Auth.YouTubeSSL, Auth.Profile |], email, privateKey)
    |> map(({ Auth.access_token }) => access_token);