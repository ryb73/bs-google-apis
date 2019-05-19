# bs-google-apis

## What is it?
A Bucklescript implementation of various Google REST APIs.

## What state is it in?
Very incomplete, unstable API. The published version may lag behind the GitHub repo – feel free to file a bug if this is the case.

## How do I install it?

```
npm i @ryb73/bs-google-apis
```

## How do I use it?

### Auth

More info:

https://developers.google.com/identity/protocols/OAuth2UserAgent
https://developers.google.com/identity/protocols/OpenIDConnect

#### getAuthUrl
Builds a URL to which you can redirect the user to begin the client-side OAuth2 flow.
```reason
let getAuthUrl:
    (~state: string=?, ~accessType: accessType=?, ~prompt: prompt=?, string,
    Js.Array.t(scope), string, responseType) => string;

GoogleApis.Auth.getAuthUrl(
    ~state="<STATE>", clientId, [| YouTube |],
    "https://my.app/", Code);
/* https://accounts.google.com/o/oauth2/v2/auth?client_id=<CLIENT_ID>&redirect_uri=https%3A%2F%2Fmy.app%2F&scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fyoutube&response_type=code&state=<STATE> */
```

#### getTokensFromCode
Retrieves access and refresh tokens given an OAuth2 code.
```reason
type tokens = {
    access_token: string,
    refresh_token: option(string),
    expires_in: int,
};

let getTokensFromCode:
    (~accessType: accessType=?, string, string, string, string) =>
    Js.Promise.t(tokens);

GoogleApis.Auth.getTokensFromCode("<client id>", "<client secret>", code, "https://my.app/");
```

#### getTokensForServiceAccount
Retrieve access and refresh tokens for a service account.
```reason
let getTokensForServiceAccount:
    (Js.Array.t(scope), string, string) => Js.Promise.t(tokens);

GoogleApis.Auth.getTokensForServiceAccount([| Profile |], "me@my.app", "<private key>");
```

#### refreshAccessToken

```reason
let refreshAccessToken:
    (string, string, string) => Js.Promise.t(tokens);

GoogleApis.Auth.refreshAccessToken(clientId, secret, refreshToken);
```

### People

More info: https://developers.google.com/people/api/rest/v1/people

#### getMe

```reason
type name = { displayName: string, };
type t = {
    resourceName: string,
    etag: string,
    names: option(array(name)),
};

let getMe: (string, Js.Array.t(field)) => Js.Promise.t(t);

GoogleApis.People.getMe(accessToken, [| Names |]);
```


### YouTube

More info: https://developers.google.com/youtube/v3/docs/

In this section you can assume `open GoogleApis.YouTube` is included.

#### parts

Represents the parts of the resource that are being requested. The parts are typed such that the type of the response you get from an API call will match the `parts` that you pass in.

```reason
let parts = PlaylistItems.(parts |> withSnippet);
PlaylistItems.listByPlaylistId(~parts, ~playlistId, accessToken);
/*
    {
        ...,
        items: [|
            ...,
            {
                id: "playlistitem123",
                contentDetails: (), /* `contentDetails` is typed as unit because `withContentDetails` was omitted */
                snippet: { /* while `snippet` is typed with the actual response data */
                    channelId: string,
                    channelTitle: string,
                    title: string,
                    description: string,
                    playlistId: string,
                    position: int,
                }
            }
        |]
    }
*/
```

#### Types

```reason
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
```

#### PlaylistItems

##### listByPlaylistId

```reason
let listByPlaylistId:
    (~maxResults: int=?, ~pageToken: string=?,
     ~parts: parts('a, 'b), ~playlistId: string, string)
    => Js.Promise.t(List.result('a, 'b));

/* (see `parts` section above for example usage) */
```

#### Playlists

##### listById

```reason
let listById:
    (~maxResults: int=?, ~parts: parts('a, 'b), ~ids: Js.Array.t(id),
    string) => Js.Promise.t(List.result('a, 'b));

Playlists.listById(~parts, ~ids=[|"<playlist ID>"|], accessToken);
```

##### listMine

List the authenticated user's playlists.

```reason
let listMine:
    (~maxResults: int=?, ~pageToken: string=?, ~parts: parts('a, 'b), string) =>
    Js.Promise.t(List.result('a, 'b));

Playlists.listMine(~parts, accessToken);
```

#### Search

##### list
```reason
let list:
    (~maxResults: int=?, ~pageToken: string=?, ~query: string, string) =>
    Js.Promise.t(List.result);

Search.list(~query, accessToken);
```

#### Videos

##### listById
```reason
let listById:
    (~maxResults: int=?, ~pageToken: string=?, ~parts: parts('a, 'b),
     ~ids: Js.Array.t(id), string) => Js.Promise.t(List.result('a, 'b));

Videos.listById(~parts, ~ids=[|"<video ID>"|], accessToken);
```
