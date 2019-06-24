open Superagent;
open ApiStd;
open YouTubeStd;
open Belt.Option;

[@decco]
type snippet = {
    channelId: string,
    channelTitle: string,
    title: string,
    description: string,
    playlistId: string,
    position: int,
};

[@decco] type id = string;

[@decco]
type contentDetails = {
    videoId: string,
    note: option(string),
};

[@decco]
type item('cd, 's) = {
    id: id,
    contentDetails: 'cd,
    snippet: 's,
};

module List = {
    [@decco]
    type result('cd, 's) = YouTubeTypes.result(item('cd, 's));

    let maxResultsLimit = 50;
};

type parts('cd, 's) = {
    string: string,
    contentDetails: Decco.decoder('cd),
    snippet: Decco.decoder('s),
};

let parts = {
    string: "",
    contentDetails: Decco.unitFromJson,
    snippet: Decco.unitFromJson,
};

let withContentDetails = (parts) => {
    ...parts,
    string: addPart(parts.string, "contentDetails"),
    contentDetails: contentDetails_decode,
};

let withSnippet = (parts) => {
    ...parts,
    string: addPart(parts.string, "snippet"),
    snippet: snippet_decode,
};

let listByPlaylistId = (~maxResults=?, ~pageToken=?, ~parts, ~playlistId, accessToken) =>
    buildGet(apiUrl, accessToken, "/playlistItems")
    |> query("part", parts.string)
    |> query("playlistId", playlistId)
    |> setOptionalQueryParam("maxResults", map(maxResults, string_of_int))
    |> setOptionalQueryParam("pageToken", pageToken)
    |> sendReq(List.result_decode(parts.contentDetails, parts.snippet));
