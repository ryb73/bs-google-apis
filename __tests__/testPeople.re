open Shared;
open Jest;
open Expect;
open Js.Promise;
open PromEx;
open GoogleApis;
open People;

testPromise("getMe", () =>
    // TODO: figure out how to test other parts
    accessToken
    |> then_(getMe(~parts=(parts |> withMetadata)))
    |> map(({ resourceName }) => expect(resourceName) |> toBe(people.userId))
);