open Superagent;
open ApiStd;
open YouTubeStd;
open Belt.Option;

[@decco.decode]
type snippet = {
    channelId: string,
    title: string,
    description: string
};

[@decco.decode] type id = string;

[@decco.decode]
type contentDetails = {
    itemCount: int,
};

[@decco.decode]
type item('cd, 's) = {
    id: id,
    contentDetails: 'cd,
    snippet: 's,
};

module List = {
    [@decco.decode]
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

let listById = (~maxResults=?, ~parts, ~ids, accessToken) =>
    buildGet(apiUrl, accessToken, "/playlists")
    |> query("part", parts.string)
    |> query("id", Js.Array.joinWith(",", ids))
    |> setOptionalQueryParam("maxResults", map(maxResults, string_of_int))
    |> sendReq(List.result_decode(parts.contentDetails, parts.snippet));

/* TODO: figure out how to test this */
let listMine = (~maxResults=?, ~pageToken=?, ~parts, accessToken) =>
    buildGet(apiUrl, accessToken, "/playlists")
    |> query("part", parts.string)
    |> query("mine", "true")
    |> setOptionalQueryParam("maxResults", map(maxResults, string_of_int))
    |> setOptionalQueryParam("pageToken", pageToken)
    |> sendReq(List.result_decode(parts.contentDetails, parts.snippet));
