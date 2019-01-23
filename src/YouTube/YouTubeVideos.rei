type snippet = {
    channelId: string,
    title: string,
    description: string,
};

type id = string;

type contentDetails = { duration: string };

type item('cd, 's) = {
    id: id,
    contentDetails: 'cd,
    snippet: 's,
};

module List: {
    type result('cd, 's) = YouTubeTypes.result(item('cd, 's));
    let maxResultsLimit: int;
};

type parts('cd, 's);
let parts: parts(unit, unit);
let withContentDetails: parts('a, 'b) => parts(contentDetails, 'b);
let withSnippet: parts('a, 'b) => parts('a, snippet);

let listById:
    (~maxResults: int=?, ~pageToken: string=?, ~parts: parts('a, 'b),
        ~ids: Js.Array.t(id), string) => Reduice.Promise.t(List.result('a, 'b));
