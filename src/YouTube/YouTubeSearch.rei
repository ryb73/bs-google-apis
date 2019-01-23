type snippet = {
    channelId: string,
    channelTitle: string,
    title: string,
    description: string,
};

type id = Video(string) | Playlist(string) | Channel(string);

type item = {
    id: id,
    snippet: snippet,
};

module List: {
    type result = YouTubeTypes.result(item);
    let maxResultsLimit: int;
};

let list:
    (~maxResults: int=?, ~pageToken: string=?, ~query: string, string) =>
    Reduice.Promise.t(List.result);
