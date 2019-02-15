open Js.Promise;

let rec autopageHelper = (~nextPageToken=?, ~maxResults, ~pageSize, acc, request) => {
    let queryMax = min(
        min(maxResults, pageSize),
        maxResults - Js.Array.length(acc)
    );
    queryMax < 1 ? resolve(acc)
    :
    request(~pageToken=?nextPageToken, queryMax)
    |> then_(({ YouTubeTypes.nextPageToken, items }) => {
        /** TODO: manage memory better */
        let acc = Js.Array.concat(items, acc);
        switch (Js.Array.length(acc) >= maxResults, nextPageToken) {
            | (false, Some(nextPageToken)) =>
                autopageHelper(~nextPageToken, ~maxResults, ~pageSize, acc, request)

            | _ => resolve(acc)
        };
    });
};

let autopage = (~maxResults=50, ~pageSize=50, f, accessToken) => {
    let request = (~pageToken=?, maxResults) =>
        /** TODO: find out why I can't just do `~maxResults,` */
        f(~maxResults=?Some(maxResults), ~pageToken?, accessToken);
    autopageHelper(~pageSize, ~maxResults, [||], request);
};
