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
        Reduice.Promise.t(tokens);
    let getTokensForServiceAccount:
        (Js.Array.t(scope), string, string) => Reduice.Promise.t(tokens);
    let refreshAccessToken:
        (string, string, string) => Reduice.Promise.t(tokens);
    let getAuthUrl:
        (~state: string=?, ~accessType: accessType=?, ~prompt: prompt=?, string,
        Js.Array.t(scope), string, responseType) => string;
};

module People: {
    [@decco] type name = { displayName: string, };
    [@decco]
    type t = {
        resourceName: string,
        etag: string,
        names: option(array(name)),
    };
    type field =
        | Addresses
        | AgeRanges
        | Biographies
        | Birthdays
        | BraggingRights
        | CoverPhotos
        | EmailAddresses
        | Events
        | Genders
        | ImClients
        | Interests
        | Locales
        | Memberships
        | Metadata
        | Names
        | Nicknames
        | Occupations
        | Organizations
        | PhoneNumbers
        | Photos
        | Relations
        | RelationshipInterests
        | RelationshipStatuses
        | Residences
        | SipAddresses
        | Skills
        | Taglines
        | Urls
        | UserDefined;
    let makeFieldsString: Js.Array.t(field) => string;
    let getMe: (string, Js.Array.t(field)) => Reduice.Promise.t(t);
};

module Types: {
    exception NoBody(Superagent.result);
    exception UnrecognizedResponseFormat(Decco.decodeError);
};

module YouTube: {
    let autopage:
        (~maxResults: int=?, ~pageSize: int=?,
          (~maxResults: int=?, ~pageToken: string=?, string) => Reduice.Promise.t(YouTubeTypes.result('b)),
          string
        ) => Reduice.Promise.t(Js.Array.t('b));
    module PlaylistItems = YouTubePlaylistItems;
    module Playlists = YouTubePlaylists;
    module Search = YouTubeSearch;
    module Videos = YouTubeVideos;
    module Types = YouTubeTypes;
};
