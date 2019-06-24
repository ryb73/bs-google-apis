[@decco.decode]
type snippet = {
    channelId: string,
    channelTitle: string,
    title: string,
    description: string,
};

[@decco.decode] type id = Video(string) | Playlist(string) | Channel(string);

[@decco.decode]
type item = {
    id: id,
    snippet: snippet,
};

module List: {
    [@decco.decode]
    type result = YouTubeTypes.result(item);
    let maxResultsLimit: int;
};

let list:
    (~maxResults: int=?, ~pageToken: string=?, ~query: string, string) =>
    Js.Promise.t(List.result);
