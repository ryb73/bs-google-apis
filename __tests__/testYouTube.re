open! GoogleApis;
open Shared;
open Jest;
open Expect;
open Js.Promise;
open PromEx;

describe("Search", () => {
    testPromise("list", () => YouTube.Search.(
        accessToken
        |> then_(list(~maxResults=1, ~query="anberlin"))
        |> map(({ YouTube.Types.items }) =>
            switch items {
            | [||] => failwith("No results found")
            | _ => expect(items[0].id) |> toEqual(Video("R4sqFmSqrSc"))
            }
        )
    ));
});

describe("Videos", () => {
    open YouTube.Videos;

    let parts = parts |> withContentDetails;

    describe("list", () => {
        testPromise("id", () =>
            accessToken
            |> then_(listById(~parts, ~ids=[| "YbJOTdZBX1g" |]))
            |> map(({ YouTube.Types.items }) =>
                switch items {
                    | [| { id } |] =>
                        id
                        |> expect |> toEqual("YbJOTdZBX1g")

                    | _ => Js.Exn.raiseError("Invalid number of results")
                }
            )
        );

        testPromise("one", () =>
            accessToken
            |> then_(listById(~parts, ~ids=[| "YbJOTdZBX1g" |]))
            |> map(({ YouTube.Types.items }) =>
                switch items {
                    | [| { contentDetails } |] =>
                        contentDetails.duration
                        |> expect |> toEqual("PT8M14S")

                    | _ => Js.Exn.raiseError("Invalid number of results")
                }
            )
        );

        testPromise("many", () =>
            accessToken
            |> then_(listById(~parts, ~ids=[| "YbJOTdZBX1g", "R4sqFmSqrSc" |]))
            |> map(({ YouTube.Types.items }) =>
                expect(Js.Array.length(items))
                |> toEqual(2)
            )
        );
    });
});

describe("Playlists", () => {
    open YouTube.Playlists;
    let parts = parts |> withSnippet;

    testPromise("list", () =>
        accessToken
        |> then_(listById(~parts, ~ids=[| "PLE62536DAAECB0527" |]))
        |> map(({ YouTube.Types.items }) =>
            switch items {
                | [| { snippet } |] =>
                    snippet.title
                    |> expect |> toEqual("Cool Experiments!")

                | _ => Js.Exn.raiseError("Invalid number of results")
            }
        )
    );
});

describe("PlaylistItems", () => {
    open YouTube.PlaylistItems;
    let parts = parts |> withSnippet;

    testPromise("list", () => {
        accessToken
        |> then_(listByPlaylistId(~parts, ~playlistId="PLE62536DAAECB0527"))
        |> map(({ YouTube.Types.items }) =>
            items[0].snippet.title
            |> expect |> toEqual("Cool Experiments -- 1")
        )
    });
});

describe("autopage", () => {
    let doTest = (maxResults, pageSize) =>
        accessToken
        |> then_(YouTube.autopage(~maxResults, ~pageSize,
            YouTube.Search.list(~query="st vincent")
        ))
        |> map(items => {
            /* Test that there are no duplicates (same page twice) */
            items
            |> Js.Array.reduce((acc, { YouTube.Search.id }) => {
                let key = Js.String.make(id);
                switch (Js.Dict.get(acc, key)) {
                    | Some(_) => Js.Exn.raiseError("Duplicate found: " ++ key)
                    | None => {
                        Js.Dict.set(acc, key, true);
                        acc;
                    }
                };
            }, Js.Dict.empty())
            |> ignore;

            /* Test that we got the expected # of items */
            Js.Array.length(items)
            |> expect |> toEqual(maxResults);
        })

    testPromise("mR == pS", () => doTest(3, 3));
    testPromise("mR > pS", () => doTest(3, 1));
    testPromise("mR < pS", () => doTest(3, 6));

    testPromise("no results", () =>
        accessToken
        |> then_(YouTube.autopage(~maxResults=10, ~pageSize=5,
            YouTube.Search.list(~query="lsdfhalsdkjfhaslkdjfhasldkfads")
        ))
        |> map(items =>
            Js.Array.length(items)
            |> expect |> toEqual(0)
        )
    );
});
