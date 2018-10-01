open Reduice.Promise;
open Belt.Result;
open Types;
open Superagent;

let apiUrl = "https://people.googleapis.com/v1";

let buildGet = (accessToken, path) =>
    get(apiUrl ++ path)
    |> setHeader(Authorization(Bearer, accessToken));

let buildPut = (accessToken, path) =>
    put(apiUrl ++ path)
    |> setHeader(Authorization(Bearer, accessToken));

let decodeResponse = (decoder, body) =>
    switch (decoder(body)) {
        | Ok(decoded) => decoded
        | Error(err) => raise(UnrecognizedResponseFormat(err))
    };

let sendReq = (decoder, request) =>
    request
    |> end_
    |> then_(({ body } as resp) =>
        switch body {
            | Some(body) => body
                |> decodeResponse(decoder)
                |> resolve
            | None => raise(NoBody(resp))
        }
    );

let req = (accessToken, path, decoder) =>
    buildGet(accessToken, path)
    |> sendReq(decoder);

let setOptionalQueryParam = (key, value, req) =>
    switch value {
        | Some(value) => query(key, value, req)
        | None => req
    };

let setOptionalParam = (key, value, req) =>
    switch value {
        | Some(value) => sendKV(key, value, req)
        | None => req
    };

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

let makeFieldsString = (fields) =>
    fields
    |> Js.Array.map((field) =>
        switch field {
            | Addresses => "addresses"
            | AgeRanges => "ageRanges"
            | Biographies => "biographies"
            | Birthdays => "birthdays"
            | BraggingRights => "braggingRights"
            | CoverPhotos => "coverPhotos"
            | EmailAddresses => "emailAddresses"
            | Events => "events"
            | Genders => "genders"
            | ImClients => "imClients"
            | Interests => "interests"
            | Locales => "locales"
            | Memberships => "memberships"
            | Metadata => "metadata"
            | Names => "names"
            | Nicknames => "nicknames"
            | Occupations => "occupations"
            | Organizations => "organizations"
            | PhoneNumbers => "phoneNumbers"
            | Photos => "photos"
            | Relations => "relations"
            | RelationshipInterests => "relationshipInterests"
            | RelationshipStatuses => "relationshipStatuses"
            | Residences => "residences"
            | SipAddresses => "sipAddresses"
            | Skills => "skills"
            | Taglines => "taglines"
            | Urls => "urls"
            | UserDefined => "userDefined"
        }
    )
    |> Js.Array.joinWith(",");

let getMe = (accessToken, fields) =>
    buildGet(accessToken, "/people/me")
    |> query("personFields", makeFieldsString(fields))
    |> sendReq(t_decode);
