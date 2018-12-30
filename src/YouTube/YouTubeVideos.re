open Superagent;
open Std;
open YouTubeStd;

[@decco.decode]
type snippet = {
    channelId: string,
    title: string,
    description: string
};

[@decco.decode] type id = string;

[@decco.decode]
type contentDetails = {
    duration: string,
};

[@decco.decode]
type item('cd, 's) = {
    id: id,
    contentDetails: 'cd,
    snippet: 's,
};

module List = {
    [@decco.decode]
    type result('cd, 's) = {
        items: array(item('cd, 's))
    };

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
    buildGet(apiUrl, accessToken, "/videos")
    |> query("part", parts.string)
    |> query("id", Js.Array.joinWith(",", ids))
    |> setOptionalQueryParam("maxResults", maxResults)
    |> sendReq(List.result_decode(parts.contentDetails, parts.snippet));
