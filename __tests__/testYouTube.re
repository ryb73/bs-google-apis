open Shared;
open Jest;
open Expect;
open Reduice.Promise;
open PromiseEx;

describe("Search", () => {
    testPromise("list", () =>
        accessToken
        |> then_(YouTube.Search.list(~query="anberlin"))
        |> map(({ YouTube.Search.List.items }) =>
            switch items {
            | [||] => failwith("No results found")
            | _ => expect(items[0].id) |> toEqual(YouTube.Search.List.Video("R4sqFmSqrSc"))
            }
        )
        |> toJs
    );
});