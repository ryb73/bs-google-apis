open Superagent;
open Std;
open YouTubeStd;

[@decco.decode]
type snippet = {
    channelId: string,
    title: string,
    description: string
};

[@decco] type id = string;

[@decco]
type contentDetails = {
    itemCount: int,
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

let listById = (~maxResults=?, ~parts, ~ids, accessToken) =>
    buildGet(apiUrl, accessToken, "/playlists")
    |> query("part", encodeParts(parts))
    |> query("id", Js.Array.joinWith(",", ids))
    |> setOptionalQueryParam("maxResults", maxResults)
    |> sendReq(List.result_decode);
