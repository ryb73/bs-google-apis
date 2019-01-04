open Superagent;
open Std;
open YouTubeStd;
open Belt.Option;

[@decco.decode]
type snippet = {
    channelId: string,
    channelTitle: string,
    title: string,
    description: string
};

[@decco.decode]
type idRaw = {
    videoId: option(string),
    playlistId: option(string),
    channelId: option(string),
};

type id = Video(string) | Playlist(string) | Channel(string);
let id_decode = (j) =>
    switch (idRaw_decode(j)) {
    | Error(_) as e => e
    | Ok({ videoId: Some(id) }) => Ok(Video(id))
    | Ok({ playlistId: Some(id) }) => Ok(Playlist(id))
    | Ok({ channelId: Some(id) }) => Ok(Channel(id))
    | Ok({ videoId: None, playlistId: None, channelId: None }) =>
        Decco.error("Invalid ID format", j)
    };

[@decco.decode]
type item = {
    id: id,
    snippet: snippet
};

module List = {
    [@decco.decode]
    type result = YouTubeTypes.result(item);

    let maxResultsLimit = 50;
};

let list = (~maxResults=?, ~nextPageToken=?, ~query as q, accessToken) =>
    buildGet(apiUrl, accessToken, "/search")
    |> query("part", "snippet")
    |> query("q", q)
    |> setOptionalQueryParam("maxResults", map(maxResults, string_of_int))
    |> setOptionalQueryParam("nextPageToken", nextPageToken)
    |> sendReq(List.result_decode);
