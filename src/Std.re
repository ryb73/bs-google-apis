open Superagent;
open Reduice.Promise;
open Belt.Result;
open Types;

let buildGet = (apiUrl, accessToken, path) =>
    get(apiUrl ++ path)
    |> setHeader(Authorization(Bearer, accessToken));

let _decodeResponse = (decoder, body) =>
    switch (decoder(body)) {
        | Ok(decoded) => decoded
        | Error(err) => raise(UnrecognizedResponseFormat(err))
    };

let sendReq = (decoder, request) =>
    end_(request)
    |> then_(({ body } as resp) =>
        switch body {
            | Some(body) => body
                |> _decodeResponse(decoder)
                |> resolve
            | None => raise(NoBody(resp))
        }
    );

let setOptionalQueryParam = (key, value, req) =>
    switch value {
        | Some(value) => query(key, value, req)
        | None => req
    };

let addPart = (string, newPart) =>
    switch string {
        | "" => newPart
        | _ => string ++ "," ++ newPart
    };