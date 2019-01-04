open Reduice.Promise;

module PlaylistItems = YouTubePlaylistItems;
module Playlists = YouTubePlaylists;
module Search = YouTubeSearch;
module Videos = YouTubeVideos;
module Types = YouTubeTypes;

let rec autopageHelper = (~nextPageToken=?, ~maxResults, ~pageSize, acc, request) => {
    let queryMax = min(
        min(maxResults, pageSize),
        maxResults - Js.Array.length(acc)
    );
    queryMax < 1 ? resolve(acc)
    :
    request(~nextPageToken?, queryMax)
    |> then_(({ Types.nextPageToken, items }) => {
        /** TODO: manage memory better */
        let acc = Js.Array.concat(items, acc);
        switch (Js.Array.length(acc) >= maxResults, nextPageToken) {
            | (false, Some(nextPageToken)) =>
                autopageHelper(~nextPageToken, ~maxResults, ~pageSize, acc, request)

            | _ => resolve(acc)
        };
    });
};

let autopage = (~maxResults=50, ~pageSize=50, f, accessToken) => {
    let request = (~nextPageToken=?, maxResults) =>
        /** TODO: find out why I can't just do `~maxResults,` */
        f(~maxResults=?Some(maxResults), ~nextPageToken?, accessToken);
    autopageHelper(~pageSize, ~maxResults, [||], request);
};
