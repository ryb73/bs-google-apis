open Superagent;
open Std;

let _apiUrl = "https://people.googleapis.com/v1";

[@decco]
type name = {
    displayName: string
};

[@decco]
type t = {
    resourceName: string,
    etag: string,
    names: option(array(name)),
};

type field = | Addresses | AgeRanges | Biographies | Birthdays | BraggingRights
    | CoverPhotos | EmailAddresses | Events | Genders | ImClients | Interests
    | Locales | Memberships | Metadata | Names | Nicknames | Occupations
    | Organizations | PhoneNumbers | Photos | Relations | RelationshipInterests
    | RelationshipStatuses | Residences | SipAddresses | Skills | Taglines | Urls
    | UserDefined;

let makeFieldsString = (fields) =>
    fields
    |> Js.Array.map((field) =>
        switch field {
            | Addresses => "addresses" | AgeRanges => "ageRanges"
            | Biographies => "biographies" | Birthdays => "birthdays"
            | BraggingRights => "braggingRights" | CoverPhotos => "coverPhotos"
            | EmailAddresses => "emailAddresses" | Events => "events"
            | Genders => "genders" | ImClients => "imClients"
            | Interests => "interests" | Locales => "locales"
            | Memberships => "memberships" | Metadata => "metadata"
            | Names => "names" | Nicknames => "nicknames"
            | Occupations => "occupations" | Organizations => "organizations"
            | PhoneNumbers => "phoneNumbers" | Photos => "photos"
            | Relations => "relations" | RelationshipInterests => "relationshipInterests"
            | RelationshipStatuses => "relationshipStatuses" | Residences => "residences"
            | SipAddresses => "sipAddresses" | Skills => "skills"
            | Taglines => "taglines" | Urls => "urls" | UserDefined => "userDefined"
        }
    )
    |> Js.Array.joinWith(",");

let getMe = (accessToken, fields) =>
    buildGet(_apiUrl, accessToken, "/people/me")
    |> query("personFields", makeFieldsString(fields))
    |> sendReq(t_decode);
