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

describe("Videos", () => {
    open YouTube.Videos;

    testPromise("list", () =>
        accessToken
        |> then_(listById(~parts=[| ContentDetails |], ~id="YbJOTdZBX1g"))
        |> map(({ List.items }) =>
            switch items {
                | [| { List.contentDetails } |] =>
                    Belt.Option.getExn(contentDetails).duration
                    |> expect |> toEqual("PT8M14S")

                | _ => failwith("Invalid number of results")
            }
        )
        |> toJs
    );
});
