module Auth: {
    [@decco]
    type tokens = {
        access_token: string,
        refresh_token: option(string),
        expires_in: int,
    };
    type responseType = Code | Token;
    type accessType = Online | Offline;
    type prompt = None | Consent | SelectAccount;
    type scope =
        | Profile
        | Email
        | YouTube
        | YouTubeSSL
        | YouTubeReadOnly
        | YouTubePartner
        | YouTubePartnerChannelAudit
        | Contacts
        | ContactsReadOnly
        | PlusLogin
        | UserAddressesRead
        | UserBirthdayRead
        | UserEmailsRead
        | UserPhoneNumbersRead
        | UserInfoEmail
        | UserInfoProfile;
    let getTokensFromCode:
        (~accessType: accessType=?, string, string, string, string) =>
        Js.Promise.t(tokens);
    let getTokensForServiceAccount:
        (Js.Array.t(scope), string, string) => Js.Promise.t(tokens);
    let refreshAccessToken:
        (string, string, string) => Js.Promise.t(tokens);
    let getAuthUrl:
        (~state: string=?, ~accessType: accessType=?, ~prompt: prompt=?, string,
        Js.Array.t(scope), string, responseType) => string;
};

module People: {
    [@decco]
    type fieldMetadata = {
        primary: bool,
        verified: bool,
    };
    [@decco] type name = {
        metadata: fieldMetadata,
        displayName: string,
    };
    [@decco] type names = array(name);
    [@decco]
    type emailAddress = {
        displayName: string,
        metadata: fieldMetadata,
        value: string,
    };
    [@decco] type emailAddresses = array(emailAddress);
    [@decco] type personMetadata = { deleted: option(bool) };
    [@decco]
    type t('e, 'm, 'n) = {
        etag: string,
        resourceName: string,
        emailAddresses: 'e,
        metadata: 'm,
        names: 'n,
    };
    type initialized;
    type uninitialized;
    type parts('i, 'e, 'm, 'n);
    let parts: parts(uninitialized, unit, unit, unit);
    let withEmailAddresses: parts('i, 'e, 'm, 'n) => parts(initialized, emailAddresses, 'm, 'n);
    let withMetadata: parts('i, 'e, 'm, 'n) => parts(initialized, 'e, personMetadata, 'n);
    let withNames: parts('i, 'e, 'm, 'n) => parts(initialized, 'e, 'm, names);
    let getMe:
        (~parts: parts(initialized, 'e, 'm, 'n), string) => Js.Promise.t(t('e, 'm, 'n));
};

module Types: {
    exception NoBody(Superagent.result);
    exception UnrecognizedResponseFormat(Decco.decodeError);
};

module YouTube: {
    let autopage:
        (~maxResults: int=?, ~pageSize: int=?,
          (~maxResults: int=?, ~pageToken: string=?, string) => Js.Promise.t(YouTubeTypes.result('b)),
          string
        ) => Js.Promise.t(Js.Array.t('b));
    module PlaylistItems = YouTubePlaylistItems;
    module Playlists = YouTubePlaylists;
    module Search = YouTubeSearch;
    module Videos = YouTubeVideos;
    module Types = YouTubeTypes;
};
