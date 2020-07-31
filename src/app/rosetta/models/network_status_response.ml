(*
 * This file has been generated by the OCamlClientCodegen generator for openapi-generator.
 *
 * Generated by: https://openapi-generator.tech
 *
 * Schema Network_status_response.t : NetworkStatusResponse contains basic information about the node's view of a blockchain network. It is assumed that any BlockIdentifier.Index less than or equal to CurrentBlockIdentifier.Index can be queried. If a Rosetta implementation prunes historical state, it should populate the optional `oldest_block_identifier` field with the oldest block available to query. If this is not populated, it is assumed that the `genesis_block_identifier` is the oldest queryable block. If a Rosetta implementation performs some pre-sync before it is possible to query blocks, sync_status should be populated so that clients can still monitor healthiness. Without this field, it may appear that the implementation is stuck syncing and needs to be terminated.
 *)

type t =
  { current_block_identifier: Block_identifier.t
  ; current_block_timestamp: Timestamp.t
  ; genesis_block_identifier: Block_identifier.t
  ; oldest_block_identifier: Block_identifier.t option [@default None]
  ; sync_status: Sync_status.t option [@default None]
  ; peers: Peer.t list }
[@@deriving yojson {strict= false}, show]

(** NetworkStatusResponse contains basic information about the node's view of a blockchain network. It is assumed that any BlockIdentifier.Index less than or equal to CurrentBlockIdentifier.Index can be queried. If a Rosetta implementation prunes historical state, it should populate the optional `oldest_block_identifier` field with the oldest block available to query. If this is not populated, it is assumed that the `genesis_block_identifier` is the oldest queryable block. If a Rosetta implementation performs some pre-sync before it is possible to query blocks, sync_status should be populated so that clients can still monitor healthiness. Without this field, it may appear that the implementation is stuck syncing and needs to be terminated. *)
let create (current_block_identifier : Block_identifier.t)
    (current_block_timestamp : Timestamp.t)
    (genesis_block_identifier : Block_identifier.t) (peers : Peer.t list) : t =
  { current_block_identifier
  ; current_block_timestamp
  ; genesis_block_identifier
  ; oldest_block_identifier= None
  ; sync_status= None
  ; peers }
