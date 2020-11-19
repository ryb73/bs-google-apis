open Superagent;
open ApiStd;
open YouTubeStd;
open Belt.Option;

[@decco.decode]
type snippet = {
    channelId: string,
    channelTitle: string,
    title: string,
    description: string
};

[@decco]
type idRaw = {
    videoId: option(string),
    playlistId: option(string),
    channelId: option(string),
};

type id = Video(string) | Playlist(string) | Channel(string);
let id_encode = fun
    | Video(s) => idRaw_encode({ videoId: Some(s), playlistId: None, channelId: None })
    | Playlist(s) => idRaw_encode({ playlistId: Some(s), videoId: None, channelId: None })
    | Channel(s) => idRaw_encode({ channelId: Some(s), playlistId: None, videoId: None });

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

let list = (~maxResults=?, ~pageToken=?, ~query as q, accessToken) =>
    buildGet(apiUrl, accessToken, "/search")
    |> query("part", "snippet")
    |> query("q", q)
    |> setOptionalQueryParam("maxResults", map(maxResults, string_of_int))
    |> setOptionalQueryParam("pageToken", pageToken)
    |> sendReq(List.result_decode);
