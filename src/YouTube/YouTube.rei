module Types: {
    type pageInfo = {
        resultsPerPage: int,
        totalResults: int,
    };

    type result('item) = {
        items: array('item),
        nextPageToken: option(string),
        prevPageToken: option(string),
        pageInfo: pageInfo,
    };
};

module PlaylistItems: {
    type snippet = {
        channelId: string,
        channelTitle: string,
        title: string,
        description: string,
        playlistId: string,
        position: int,
    };

    type id = string;

    type contentDetails = {
        videoId: string,
        note: option(string),
    };

    type item('cd, 's) = {
        id: id,
        contentDetails: 'cd,
        snippet: 's,
    };

    module List: {
        type result('cd, 's) = Types.result(item('cd, 's));
        let maxResultsLimit: int;
    };

    type parts('cd, 's);
    let parts: parts(unit, unit);
    let withContentDetails: parts('a, 'b) => parts(contentDetails, 'b);
    let withSnippet: parts('a, 'b) => parts('a, snippet);

    let listByPlaylistId:
        (~maxResults: int=?, ~pageToken: string=?,
            ~parts: parts('a, 'b), ~playlistId: string, string)
        => Reduice.Promise.t(List.result('a, 'b));
};

module Playlists: {
    type snippet = {
        channelId: string,
        title: string,
        description: string,
    };

    type id = string;

    type contentDetails = {
        itemCount: int,
    };

    type item('cd, 's) = {
        id: id,
        contentDetails: 'cd,
        snippet: 's,
    };

    module List: {
        type result('cd, 's) = Types.result(item('cd, 's));
        let maxResultsLimit: int;
    };

    type parts('cd, 's);
    let parts: parts(unit, unit);
    let withContentDetails: parts('a, 'b) => parts(contentDetails, 'b);
    let withSnippet: parts('a, 'b) => parts('a, snippet);

    let listById:
        (~maxResults: int=?, ~parts: parts('a, 'b), ~ids: Js.Array.t(id),
        string) => Reduice.Promise.t(List.result('a, 'b));

};

module Search: {
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
        type result = Types.result(item);
        let maxResultsLimit: int;
    };

    let list:
        (~maxResults: int=?, ~pageToken: string=?, ~query: string, string) =>
        Reduice.Promise.t(List.result);

};

module Videos: {
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
        type result('cd, 's) = Types.result(item('cd, 's));
        let maxResultsLimit: int;
    };

    type parts('cd, 's);
    let parts: parts(unit, unit);
    let withContentDetails: parts('a, 'b) => parts(contentDetails, 'b);
    let withSnippet: parts('a, 'b) => parts('a, snippet);

    let listById:
        (~maxResults: int=?, ~pageToken: string=?, ~parts: parts('a, 'b),
            ~ids: Js.Array.t(id), string) => Reduice.Promise.t(List.result('a, 'b));
};

let autopage:
    (~maxResults: int=?, ~pageSize: int=?,
        (~maxResults: int=?, ~pageToken: string=?, string) =>
            Reduice.Promise.t(Types.result('b))
    , string) => Reduice.Promise.t(Js.Array.t('b));
