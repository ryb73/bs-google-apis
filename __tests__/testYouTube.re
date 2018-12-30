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
            | _ => expect(items[0].id) |> toEqual(YouTube.Search.Video("R4sqFmSqrSc"))
            }
        )
        |> toJs
    );
});

describe("Videos", () => {
    open YouTube.Videos;

    describe("list", () => {
        testPromise("one", () =>
            accessToken
            |> then_(listById(~parts=[| ContentDetails |], ~ids=[| "YbJOTdZBX1g" |]))
            |> map(({ List.items }) =>
                switch items {
                    | [| { contentDetails } |] =>
                        Belt.Option.getExn(contentDetails).duration
                        |> expect |> toEqual("PT8M14S")

                    | _ => Js.Exn.raiseError("Invalid number of results")
                }
            )
            |> toJs
        );

        testPromise("many", () =>
            accessToken
            |> then_(listById(
                ~parts=[| ContentDetails |],
                ~ids=[| "YbJOTdZBX1g", "R4sqFmSqrSc" |],
            ))
            |> map(({ List.items }) =>
                expect(Js.Array.length(items))
                |> toEqual(2)
            )
            |> toJs
        );
    });
});

describe("Playlists", () => {
    open YouTube.Playlists;

    testPromise("list", () =>
        accessToken
        |> then_(listById(~parts=[| Snippet |], ~ids=[| "PLE62536DAAECB0527" |]))
        |> map(({ List.items }) =>
            switch items {
                | [| { snippet } |] =>
                    Belt.Option.getExn(snippet).title
                    |> expect |> toEqual("Cool Experiments!")

                | _ => Js.Exn.raiseError("Invalid number of results")
            }
        )
        |> toJs
    );
});
