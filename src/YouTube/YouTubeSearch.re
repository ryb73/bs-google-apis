open Superagent;
open Std;
open YouTubeStd;

[@decco.decode]
type snippet = {
    channelId: string,
    channelTitle: string,
    title: string,
    description: string
};

[@decco.decode]
type _idRaw = {
    videoId: option(string),
    playlistId: option(string),
    channelId: option(string),
};

type id = Video(string) | Playlist(string) | Channel(string);
let id_decode = (j) =>
    switch (_idRaw_decode(j)) {
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
    type result = {
        items: array(item)
    };

    let maxResultsLimit = 50;
};

let list = (~maxResults=?, ~query as q, accessToken) =>
    buildGet(apiUrl, accessToken, "/search")
    |> query("part", "snippet")
    |> query("q", q)
    |> setOptionalQueryParam("maxResults", maxResults)
    |> sendReq(List.result_decode);
