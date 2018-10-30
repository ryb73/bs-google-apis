open Superagent;
open Std;

let _apiUrl = "https://www.googleapis.com/youtube/v3";

module List = {
    [@decco]
    type snippet = {
        channelId: string,
        channelTitle: string,
        title: string,
        description: string
    };

    [@decco]
    type id = {
        videoId: string
    };

    [@decco]
    type item = {
        id: id,
        snippet: snippet
    };

    [@decco]
    type items = {
        items: array(item)
    };

    [@decco]
    type result = {
        result: items
    };

    let maxResultsLimit = 50;
};

let _setOptionalQueryParam = (key, value, req) =>
    switch value {
        | Some(value) => query(key, value, req)
        | None => req
    };

let list = (~maxResults=?, ~part, ~query as q, accessToken) =>
    buildGet(_apiUrl, accessToken, "/search")
    |> query("part", part)
    |> query("q", q)
    |> _setOptionalQueryParam("maxResults", maxResults)
    |> sendReq(result_decode);