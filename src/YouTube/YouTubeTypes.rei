[@decco]
type pageInfo = {
    resultsPerPage: int,
    totalResults: int,
};

[@decco]
type result('item) = {
    items: array('item),
    nextPageToken: option(string),
    prevPageToken: option(string),
    pageInfo: pageInfo,
};
