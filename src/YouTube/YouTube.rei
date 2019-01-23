let autopage:
    (~maxResults: int=?, ~pageSize: int=?,
        (~maxResults: int=?, ~pageToken: string=?, string) =>
            Reduice.Promise.t(YouTubeTypes.result('b))
    , string) => Reduice.Promise.t(Js.Array.t('b));
