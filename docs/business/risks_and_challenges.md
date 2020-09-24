# Risks & Challenges Analysis

**Table of Contents:**

1. [Uncontrollable Forces](#uncontrollable-forces)
2. [Imposed Constraints](#imposed-constraints)
3. [Difficult or Risky Value Propositions](#difficult-or-risky-value-propositions)
4. [What could go wrong?](#what-could-go-wrong)
5. [Customer Concerns](#customer-concerns)
6. [Risk Evaluation](#risk-evaluation)
7. [Risk Management](#risk-management)

## Uncontrollable Forces

* Vendor SDK release quality
* People's feeling towards vendor SDKs (e.g., free, love to use them, think they are garbage)
* Nobody cares about our logic or savings in time/money areas
* Preconcieved notions our clients have about vendors, external IP, paying for software, consultants
* Client migrating to a new processor not in our supported parts list
* The stage in the product development lifecycle a company is at when we meet them
    * Want super early (pre-development) or onto product 2/3 (mature)
* The processor and components produced by hardware vendors
    * Parts going end-of-life, new cores getting released, IP fragmentation, etc.
* Change in security requirements/legislation for embedded devices

## Imposed Constraints

* Not hiring additional staff (EA) - time + support + dev + sales constraint
* Focus on baremetal systems
* Focus on C++
* Available hardware offerings
* Chips/components selected by client companies

## Difficult or Risky Value Propositions

1. Defining an RTOS abstraction that covers a wide variety of RTOSes
2. Processor abstraction - across families & architectures
3. Supporting enough chips + drivers to make the schedule reduction claims worthwhile
4. Keeping the framework simple (i.e., the framework is at risk of becoming an expert system)
5. Getting generic interfaces right for all modules
6. Making it easy to build subsets of software in a host machine scenario

## What could go wrong?

* Build ineffective/bad/incomplete abstractions:
	* RTOS, processor, architecture, chip family, HAL, Driver base class
* Build bad/incomplete documentation
* Nobody wants to license the framework
* Can't keep up with support and bringup needs
* Add unacceptable latency to firmware
* Resulting firmware size is too large
* Build an expert system
* Adding support for a board + all new drivers takes too long for a client to see the value
* Our release has a critical bug - making the releas unusable or leaving a client's product at risk of a security issue
* We ignore a critical use case, causing us to be functionally limited. For example, is it true that we don't need:
	* supporting multiple platforms in one image
	* supporting two processors on one platform with one image (only possible if they are the same architecture)

## Customer Concerns

* CEO
    * Does this get me to my products faster?
    * Is this too expensive?
    * Will my team understand it?
    * Does this help me produce more value for my customers?
* Experienced Developer
    * Do I have the ability to customize this to my needs and break out of the simple interfaces?
    * How long will this take me to learn?
* C developer
    * How much C++ do I need to learn to be useful on this project?
* Inexperienced developer
    * How long will this take me to learn?
    * Are there enough examples for me to figure out how to use the framework?
    * Are these guides to help me get started?
    * How can I learn about these more advanced concepts?

## Risk Evaluation

|    Risk Name    | Impact Rating | Likelihood of Occurrence | Detection Difficulty | Overall Rating |
| --------------- | ------------- | ------------------------ | -------------------- | -------------- |
| **[Bad Abstractions](#bad-abstractions)** | H | H | M | H |
| [Bad Documentation](#bad-documentation) | M | L | L | L |
| **[No Licensees](#no-licensees)** | M | M | L | M |
| **[Add Unacceptable Latency](#add-unacceptable-latency)** | M | M | M | M |
| **[Big Binary Size](#big-binary-size)** | H | M | L | M |
| **[Expert System](#expert-system)** | H | M | H | H |
| **[Critical Bug in Release](#critical-bug-in-release)** | H | M | L | M |
| [Missing Use Case](#missing-use-case) | M | L | L | L |
| **[Can't handle support requirements](#cant-handle-support-requirements)** | H | M | L | M |
| [Can't handle new part bring-up](#cant-handle-new-part-bring-up) | M | L | L | L |
| [Legislation Change](#legislation-change) | H | L | L | M |

### Key

* H = high, M = medium, l = low
* Impact: H/M/L based on perceived impact level
* Likelihood of occurrence: H = 50-100%, M = 10-50%, L = 0-10%
* Detection difficulty: H = difficult to find, L = easy to find
* Overall rating: What is your rating of the overall risk? (H/M/L)

## Risk Management

Primary risks are:

1. [Bad Abstractions](#bad-abstractions)
2. [Add Unacceptable Latency](#add-unacceptable-latency)
3. [Big Binary Size](#big-binary-size)
4. [Expert System](#expert-system)
5. [Critical Bug in Release](#critical-bug-in-release)

Secondary risks are:

1. [No Licensees](#no-licensees)
2. [Can't handle support requirements](#cant-handle-support-requirements)
3. [Can't handle new part bring-up](#cant-handle-new-part-bring-up)

Tertiary risks are:

1. [Bad Documentation](#bad-documentation)
2. [Missing Use Case](#missing-use-case)
3. [Legislation Change](#legislation-change)

### Bad Abstractions

Avoidance strategies:

1. Research existing abstractions for inspiration and santiy checks
2. Test the abstractions by analyzing multiple architectures, component families, and processors
3. Perform at least 3 drafts on interfaces with at least 1-2 days between

Mitigation Strategies:

1. Make each abstraction extendable - users can adjust to their needs, each chip can provide custom support
2. Find ways to extend functionality while preserving existing APIs to minimize impact

Contingency strategies:

1. Refactor the bad parts to provide simplified interfaces and use models
2. Provide adapters for existing clients who are depending on bad abstractions
    1. They won't need to migrate, but the fundamental APIs can change
3. Help our clients migrate their code through difficult/breaking API changes

### Add Unacceptable Latency

Avoidance strategies:

1. Define latency metrics and measure them across builds
2. Minimize layers of abstraction whever possible

Mitigation Strategies:

1. Provide workarounds that bypass latency-adding paths so users with latency requirements can meet them

Contingency strategies:

1. Identify high-latency areas and provide workarounds or abstraction-breaking mechanisms to accomplish task
1. Hire outside help to reduce latency & optmize framework

### Big Binary Size

Avoidance strategies:

1. Monitor binary and library sizes in each build (CI Server)
2. Compare binary size to other systems to sanity check order of magnitude output

Mitigation Strategies:

1. Provide ability to control optimization and debug settings
2. Keep debugging off in the framework unless a client needs it
3. Turn off features that aren't needed

Contingency strategies:

1. Request client goes to larger flash/RAM
2. Disable debugging
3. Hire outside help to help reduce binary size

### Expert System

Avoidance strategies:

1. Review interfaces and ideas with external developers throughout the development process
2. Test the APIs by building drivers and systems to identify tricky places
3. Perform at least 3 drafts on interfaces with at least 1-2 days between - focus on simplicity
4. Explain operational models to Rozi and ensure she understands them

Mitigation Strategies:

1. Provide documentation and examples to help users understand
2. Provide support and training to clients

Contingency strategies:

1. Refactor the complicated parts to provide simplified interfaces and use models

### Critical Bug in Release

Avoidance strategies:

1. Maintain a test server + run unit tests daily
2. Perform static analysis to identify bugs before they are fixed
3. Create a review checklist before a release can be made

Mitigation Strategies:

1. Use long-term stability branches to keep clients isolated from rapid changes
2. Perform a slow roll-out: deploy to one client at a time and make sure there are no major problems

Contingency strategies:

1. Provide immediate service to get the bug fixed with minimal delay

### No Licensees

Avoidance strategies:

1. Find 1-2 companies who want to license the framework while it is still in development
2. Interview companies
3. Find the right value propositions for each business segment

Mitigation Strategies:

1. Don't "license" framework, provide the framework at noce cost and client pays for setup + support (optional)
2. Create a trial license period

Contingency strategies:

1. Don't tell the clients about the framework, just use the IP to complete projects faster

### Can't Handle Support Requirements

Avoidance strategies:

1. Provide quality documentation
2. Provide examples as reference
3. Set up the framework for the client
4. Record videos showing common tasks

Mitigation Strategies:

1. Train Rozi to help with support
2. Create a forum to archive questions/answers
3. Create an FAQ

Contingency strategies:

1. Hire contractors to help with support
2. Hire interns to help with support
3. Pay Anthony to help on the side

### Can't Handle New Part Bring-up

Avoidance strategies:

1. Identify components that first 1-2 clients are using and develop those first
2. Bill for developing new components during the bring-up phase

Mitigation Strategies:

1. Continually ask people what parts they are using, make a list of components & rank by overlaps
2. Download another driver and put an adapter interface until we can get code working
3. Use vendor SDK underneath framework APIs and port after bring-up

Contingency strategies:

1. Hire contractors to assist with bring-up
2. Hire intern(s) to help write and test new drivers
3. Pay Anthony to help read datasheets and write drivers - good EE experience!

### Bad Documentation

Avoidance strategies:

1. Document as you go
2. Capture rationale
3. Review documentation along the way with Rozi and reviewers

Mitigation Strategies:

1. Provide example code to demonstrate framework usage, even if documentation is lacking
2. Supplement documentation with email/forum support
3. Provide videos covering framework usage

Contingency strategies:

1. Provide direct support to client and adjust documentation when missing pieces are identified

### Missing Use Case

Avoidance strategies:

1. Review use cases, scope, and feature with developers during architecture phase
2. Talk to developers and companies about their projects and requirements

Mitigation Strategies:

1. Ensure that the framework provides only the minimal set of features so that use cases aren't limited
2. Develop each piece of the framework so that it is easily extended

Contingency strategies:

1. Add a missing use case in as a standalone feature / subsystem


### Legislation Change

Avoidance strategies:

1. N/A

Mitigation Strategies:

1. Stay up-to-date on security standards and adhere to them
2. Perform static analysis and testing to ensure bugs are eliminated

Contingency strategies:

1. Adjust to legsilation change
2. Purchase compliant software

