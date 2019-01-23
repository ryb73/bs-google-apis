type pageInfo = {
    resultsPerPage: int,
    totalResults: int,
};

[@decco.decode]
type result('item) = {
    items: array('item),
    nextPageToken: option(string),
    prevPageToken: option(string),
    pageInfo: pageInfo,
};
