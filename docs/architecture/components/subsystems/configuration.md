# CRC-R: Configuration Subsystem

AKA KV-Store

## Responsibilities

TBD

## Requirements

TBD

## Collaborators

TBD

## Rationale

TBD

## Source Links

TBD

## Notes

Two models:

* RAM-Based KV store
	* Resets every boot, but you can change default parameters for testing
* Flash-based KV store
	* Permanent changes until re-flashed

Something most projects need:
* persistent KV storage with nvram/flash
* definitely an add-on module
