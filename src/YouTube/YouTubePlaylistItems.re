open Superagent;
open Std;
open YouTubeStd;

[@decco.decode]
type snippet = {
    channelId: string,
    channelTitle: string,
    title: string,
    description: string,
    playlistId: string,
    position: int,
};

[@decco.decode] type id = string;

[@decco.decode]
type contentDetails = {
    videoId: string,
    note: option(string),
};

[@decco.decode]
type item = {
    id: id,
    contentDetails: option(contentDetails),
    snippet: option(snippet),
};

module List = {
    [@decco.decode]
    type result = {
        items: array(item)
    };

    let maxResultsLimit = 50;
};

type part = ContentDetails | Id | Snippet;
let encodePart = fun
    | ContentDetails => "contentDetails" | Id => "id" | Snippet => "snippet";

let encodeParts = (parts) =>
    Js.Array.map(encodePart, parts)
    |> Js.Array.joinWith(",");

let listByPlaylistId = (~maxResults=?, ~parts, ~playlistId, accessToken) =>
    buildGet(apiUrl, accessToken, "/playlistItems")
    |> query("part", encodeParts(parts))
    |> query("playlistId", playlistId)
    |> setOptionalQueryParam("maxResults", maxResults)
    |> sendReq(List.result_decode);
