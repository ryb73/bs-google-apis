let autopage:
    (~maxResults: int=?, ~pageSize: int=?,
        (~maxResults: int=?, ~pageToken: string=?, string) =>
            Js.Promise.t(YouTubeTypes.result('b))
    , string) => Js.Promise.t(Js.Array.t('b));
