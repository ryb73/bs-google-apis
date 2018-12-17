open Superagent;
open Std;
open YouTubeStd;

module List = {
    [@decco.decode]
    type snippet = {
        channelId: string,
        title: string,
        description: string
    };

    [@decco] type id = string;

    [@decco]
    type contentDetails = {
        duration: string,
    };

    [@decco.decode]
    type item = {
        id: id,
        contentDetails: option(contentDetails),
        snippet: option(snippet),
    };

    [@decco.decode]
    type result = {
        items: array(item)
    };

    let maxResultsLimit = 50;
};

type part =
    | ContentDetails | FileDetails | Id | LiveStreamingDetails | Localizations
    | Player | ProcessingDetails | RecordingDetails | Snippet | Statistics
    | Status | Suggestions | TopicDetails;

let encodePart = fun
    | ContentDetails => "contentDetails" | FileDetails => "fileDetails" | Id => "id"
    | LiveStreamingDetails => "liveStreamingDetails" | Localizations => "localizations"
    | Player => "player" | ProcessingDetails => "processingDetails"
    | RecordingDetails => "recordingDetails" | Snippet => "snippet"
    | Statistics => "statistics" | Status => "status" | Suggestions => "suggestions"
    | TopicDetails => "topicDetails";

let encodeParts = (parts) =>
    Js.Array.map(encodePart, parts)
    |> Js.Array.joinWith(",");

let listById = (~maxResults=?, ~parts, ~id, accessToken) =>
    buildGet(apiUrl, accessToken, "/videos")
    |> query("part", encodeParts(parts))
    |> query("id", id)
    |> setOptionalQueryParam("maxResults", maxResults)
    |> sendReq(List.result_decode);
