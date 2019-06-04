open Superagent;
open ApiStd;

let _apiUrl = "https://people.googleapis.com/v1";

[@decco]
type fieldMetadata = {
    primary: bool,
    verified: bool,
};

[@decco]
type emailAddress = {
    displayName: string,
    metadata: fieldMetadata,
    value: string,
};

[@decco] type emailAddresses = option(array(emailAddress));

[@decco] type personMetadata = { deleted: option(bool) };

[@decco]
type name = {
    metadata: fieldMetadata,
    displayName: string
};

[@decco] type names = array(name);

[@decco]
type t('e, 'm, 'n) = {
    etag: string,
    resourceName: string,
    emailAddresses: 'e,
    metadata: 'm,
    names: 'n,
};

/* The `initialized` field will be `unit` when uninitialized
    and `int` when initialized */
type initialized = int;
type uninitialized = unit;
type parts('i, 'e, 'm, 'n) = {
    initialized: 'i,
    personFields: string,
    emailAddresses: Decco.decoder('e),
    metadata: Decco.decoder('m),
    names: Decco.decoder('n),
};

let parts = {
    initialized: (),
    personFields: "",
    emailAddresses: Decco.unitFromJson,
    metadata: Decco.unitFromJson,
    names: Decco.unitFromJson,
};

let withEmailAddresses = (parts) => {
    ...parts,
    initialized: 1,
    personFields: addPart(parts.personFields, "emailAddresses"),
    emailAddresses: emailAddresses_decode,
};

let withMetadata = (parts) => {
    ...parts,
    initialized: 1,
    personFields: addPart(parts.personFields, "metadata"),
    metadata: personMetadata_decode,
};

let withNames = (parts) => {
    ...parts,
    initialized: 1,
    personFields: addPart(parts.personFields, "names"),
    names: names_decode,
};

let getMe = (~parts, accessToken) => {
    let { personFields, emailAddresses, metadata, names } = parts;
    buildGet(_apiUrl, accessToken, "/people/me")
    |> query("personFields", personFields)
    |> sendReq(t_decode(emailAddresses, metadata, names));
};
