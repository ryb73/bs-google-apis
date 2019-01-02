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

    let parts = parts |> withContentDetails;

    describe("list", () => {
        testPromise("id", () =>
            accessToken
            |> then_(listById(~parts, ~ids=[| "YbJOTdZBX1g" |]))
            |> map(({ List.items }) =>
                switch items {
                    | [| { id } |] =>
                        id
                        |> expect |> toEqual("YbJOTdZBX1g")

                    | _ => Js.Exn.raiseError("Invalid number of results")
                }
            )
            |> toJs
        );

        testPromise("one", () =>
            accessToken
            |> then_(listById(~parts, ~ids=[| "YbJOTdZBX1g" |]))
            |> map(({ List.items }) =>
                switch items {
                    | [| { contentDetails } |] =>
                        contentDetails.duration
                        |> expect |> toEqual("PT8M14S")

                    | _ => Js.Exn.raiseError("Invalid number of results")
                }
            )
            |> toJs
        );

        testPromise("many", () =>
            accessToken
            |> then_(listById(~parts, ~ids=[| "YbJOTdZBX1g", "R4sqFmSqrSc" |]))
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
    let parts = parts |> withSnippet;

    testPromise("list", () =>
        accessToken
        |> then_(listById(~parts, ~ids=[| "PLE62536DAAECB0527" |]))
        |> map(({ List.items }) =>
            switch items {
                | [| { snippet } |] =>
                    snippet.title
                    |> expect |> toEqual("Cool Experiments!")

                | _ => Js.Exn.raiseError("Invalid number of results")
            }
        )
        |> toJs
    );
});

describe("PlaylistItems", () => {
    open YouTube.PlaylistItems;

    testPromise("list", () => {
        let parts = parts |> withSnippet;

        accessToken
        |> then_(listByPlaylistId(~parts, ~playlistId="PLE62536DAAECB0527"))
        |> map(({ List.items }) =>
            items[0].snippet.title
            |> expect |> toEqual("Cool Experiments -- 1")
        )
        |> toJs
    });
});
