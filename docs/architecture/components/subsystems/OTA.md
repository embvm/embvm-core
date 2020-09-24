# CRC-R: Over-the-Air Updates

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

* OTA Module:
    * need 2-stage boot + firmware + recovery FW (or backup full firmware depending on flash size)
    * must meet minimal flash size requirements!

They will need a dependency matrix to know what software can work with what other software 

In case updates fail, they might need a rollback to get to the correct version to try another update

Rollback support 


For a project this complicated, I can’t just roll up your sleeves and get to work. It needs time designing the solution. If it won’t work on paper, it certainly won’t work in code.

I’ve seen OTA delay the ship date for more projects than I can keep track of. Especially when it’s started last. It needs to be high reliability.

There are a lot of things to answer:

How does the update get to the customer? Phone?
What is the payload?
What are the image formats?
Are they signed? Encrypted? How will they be validated? Will the devices decrypt them, or will the phone?
What happens when an update fails? Is there fallback firmware, a second image in a ping pong buffer, does the system just break?
What happens if one update fails, but the rest worked? How will the system know? What steps will be taken?
Will you track a dependency matrix of image versions? I imagine there are comm protocol changes or other software causes that will make versions have dependencies on each other
What happens when a single update fails, and now there is a dependency mismatch? Do you downgrade and try again?
How will you handle downgrades? (This happens - more than you think)

What happens if an update fails altogether? Can a customer put the device in DFu mode and manually recover with a cable? Or do they send it in for repair?
How many repair cycles will you handle before losing money?
Will you ship out a replacement immediately?

Will you have a zero-day update?
Will you ship a minimal version that just receives updates?
