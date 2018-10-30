open Jest;
open Expect;

test("getAuthUrl", () =>
    Auth.getAuthUrl(~state="stayt", "cli", [|Auth.YouTube|],
        "http://www.com/", Auth.Code)
    |> expect
    |> toBe("https://accounts.google.com/o/oauth2/v2/auth?client_id=cli&redirect_uri=http%3A%2F%2Fwww.com%2F&scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fyoutube&response_type=code&state=stayt")
);
