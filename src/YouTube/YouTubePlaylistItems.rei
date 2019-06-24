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

module List: {
    [@decco]
    type result('cd, 's) = YouTubeTypes.result(item('cd, 's));
    let maxResultsLimit: int;
};

type parts('cd, 's);
let parts: parts(unit, unit);
let withContentDetails: parts('a, 'b) => parts(contentDetails, 'b);
let withSnippet: parts('a, 'b) => parts('a, snippet);

let listByPlaylistId:
    (~maxResults: int=?, ~pageToken: string=?,
        ~parts: parts('a, 'b), ~playlistId: string, string)
    => Js.Promise.t(List.result('a, 'b));
