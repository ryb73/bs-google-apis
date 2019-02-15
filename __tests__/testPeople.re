open Shared;
open Jest;
open Expect;
open Js.Promise;
open PromEx;

testPromise("getMe", () =>
    accessToken
    |> then_(People.getMe(_, [| People.Names |]))
    |> map(({ People.resourceName }) => expect(resourceName) |> toBe(people.userId))
);