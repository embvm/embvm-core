Version Description Document

There's a template in icloud drive

<pre>
Implementing a Version Description Document

By  , January 01, 1996
Post a  Comment
  One of the major nightmares of software product maintenance is determining what version you have in your possession. Software developers pay a lot of attention to configuration management at the code level; however, this is only one part of the configuration management story. Customer releases, both external and internal, are another integral part of configuration management and maintenance systems.
While automated tools facilitate consistent configuration management and maintenance environments, these tools usually benefit the people producing the product; the people receiving the product are often forgotten.
Any tester (and believe me, if you handle software at any stage of the development process you are a tester) can relate horror stories about a developer handing over a release disk and stating, with profuse assurances, that this is the final cut of the product. After a while, these "final" releases start cluttering your desktop and make it difficult to find the disk with the latest "final" version. Many companies have a good handle on making interim releases, but many others do not.
This article describes a method for tracking interim releases called a Version Description Document (VDD). The VDD described in this article began within a systems testing group I was involved with several years ago. Our group addressed maintenance and testing problems created when products from distinct groups were combined into a single, cohesive system. All the products had to work with each other, so we needed to keep track of all release revisions.
But tracking these revisions wasn't easy because we received releases from several development groups at the same time. We developed the VDD as a solution and used the document internally within our group. We required any development team that wanted to place a product in our system test to produce a completed VDD. Because our group had to sign off on all products before they could be released, the VDD soon became a company-wide procedure.
The Reasons Behind a VDD

The basis for the VDD developed here is provided in the Dept. of Defense document DI-MCCR-80013. Before we proceed any further, a definition of a VDD is in order. The VDD provides descriptive information on proposed product revisions submitted to any customer, whether internal or external. Submitting VDDs facilitates test planning, product maintenance, and product utility.
The key point here is the term "customer." When a development team releases a version of a product to internal groups such as testers and developers or to external groups such as strategic partners and contractors, these "customers" must know exactly what they are receiving. The final product release will most certainly be accompanied by the appropriate documentation and release notes. However, this is usually not the case with interim releases.
Uncontrolled interim releases indicate a severe lack of discipline among development team members. Too many developers do not track their releases or document the processes they use to develop a particular release. This lack of discipline will most likely rear its ugly head in other areas. A VDD not only helps a customer, it forces the development team to become more methodical and pay more attention to its own processes and procedures.
While the developer reaps benefits from the VDD, internal and external customers reap the most benefits. For example, the VDD provides management with a good vehicle for tracking what is actually going on with product development, such as how many releases are being produced and what type of bugs are cropping up. The VDD must be signed off by management, which provides another incentive for developers to solidify their release process, test the changes thoroughly, and keep the number of revisions to a minimum.
The VDD applies to any release of a product revision, which includes not only software, but hardware and firmware as well. While software may be the primary focus in some organizations (compilers, spreadsheets, and so on), a great deal of software is bundled into products and systems that include hardware and firmware. Such environments include more variables, which makes a VDD even more vital to the maintenance of a product or system.
The most important information the VDD contains is the approved product name, identification number (catalogue number, part number), and release version. Each time a developer revises the product, a VDD must accompany the release. For example, a tester should not accept a release for testing unless it is packaged with a VDD. Developers new to writing VDDs will usually balk at complying with this condition--at least at the beginning. In the end, most developers see the utility of the document. Producing a VDD is not incredibly time consuming, but it isn't trivial either--this fact is another incentive for developers to keep releases to a minimum.
The VDD Defined

You can record a variety of information on a VDD. The actual format depends on the needs of your organization. The following outline describes a document that has been proven in a production environment. These steps are shown in Figure 1.
I. Scope

The VDD applies to all areas of the organization that submit releases of computer software or firmware to internal or external customers for testing of computer software or firmware. All releases must be accompanied by a completed VDD signed by the appropriate management and development authorities. Other information contained in the Scope section includes:
a) Identification: This paragraph contains the approved product name, identification number (catalogue number, part number, and so on), and version.
b) Purpose: The VDD provides descriptive information on product revisions submitted to any internal or external customer for evaluation. Submitting VDDs facilitates test planning and product maintenance.
II. References

Any references or other sources used in developing the VDD must be cited in this paragraph.
III. Version Description

The information contained in the version description is described in the following sections:
a) Inventory of Materials Released: Every release has a physical aspect to it. For a software product, the executable (assuming that is the released form) is distributed over some form of media, such as a 3 1/2-inch floppy disk or perhaps a CD-ROM. The distribution may even be made electronically via e-mail, a forum, or a bulletin board. Along with the executable, some supporting documentation may be needed to operate the program properly. Regardless of the manner used to build or distribute the software, this paragraph must describe all the physical material or media included in the distribution package.
b) Inventory of Software/Firmware Released: Besides the physical material, the software release must be specifically identified. This paragraph identifies, by version number, all computer software and firmware being released. This information is critical when tracking software during testing and maintenance. In many cases, requiring this information may impose a more disciplined configuration management system policy within the organization.
c) Enhancements to Functionality: In many environments, new features come flowing out of the development groups as fast as they are coded and as long as time permits before deadline. Describing these enhancements is extremely important to the people doing the testing and maintenance so they can effectively track the enhancements. This paragraph identifies all new functionality since the previous release. Be specific about the changes made to the product. Information of this kind helps testers target areas of code that have changed and need more rigorous testing. Much of this information is also relevant to other members of the organization, such as marketing and management, where functionality is tracked closely.
d) Maintenance to Product: Bug fixes, performance issues, and other maintenance are reasons to change code and produce a new release. Unfortunately, bugs are unavoidable, and at times accumulate at a rapid pace. Many bugs are quick to fix and each fix creates a new version of code. This paragraph identifies and describes all maintenance that has been performed on the product since the previous release. You may want to consider accumulating several bugs before you make another release to reduce the number of total revisions. If a bug brings some part of testing to a halt, you will need to fix the bug as soon as possible. All such changes are recorded in this paragraph.
e) Adaptation Data: Sometimes a product behaves differently in different environments. This paragraph identifies any unique-to-site data. For example, code may behave differently on a certain PC or on different networks. Any issue requiring special attention due to the environment must be listed in this paragraph.
f) Interface Compatibility: Most programs do not operate independently. In many cases, software interacts with several different products. This paragraph identifies all other products or systems that are affected by any change made to the product. This may include other software, such as drivers, or even other hardware, such as cards and peripherals. This information is crucial because these changes can create bugs that are almost impossible to track down.
IV. Installation Instructions

This paragraph may be the most obviously necessary, yet the most ignored. Virtually all people know the feeling of buying a new product and having insufficient or incorrect assembly or installation instructions. This section contains the information necessary for installing the product. It is amazing how many times a new software product arrives on a desk with simply a floppy disk and nothing else. With the standardization of install and setup programs, it is somewhat easier to install code. But what about changes to the programming environment, hardware changes, or myriad other possibilities? This paragraph must contain the answers to such questions.
V. Deinstallation Instructions

This section provides instructions for deinstalling a product version. This information is important for two reasons. First, when installing a new revision, some procedures may be required to remove the old revision. Second, if the installation of the new revision fails, there must be a way to restore the old revision to a working state. Restoring the old version may not always be possible. However, contingencies must be made for this situation.
VI. Completion of Previous Testing

In many stages of development, the completion of previous testing comes into play. First, if the release is made to a functional test group, then the unit testing must already be complete. Likewise, if the release is made to a system test group, then functional testing must be complete.
In system testing, for example, a development group is often hurried by the impending ship date and will release code to system test without fully completing the functional tests. While some groups release a product prematurely to expedite the process, in many cases they're also hoping that any functional issues that slip through will be found in system test. This way of thinking is not healthy! If you make sure all previous testing is complete, you will save people work down the road and make your process more disciplined.
VII. Possible Problems, Known Bugs, and Anomalies

No matter how good a product is, there will always be bugs. Your goal should be to identify and fix all bugs. However, in real life this goal may not be attainable. This section presents all known anomalies, potential problems and, if available, all work-arounds.
If known problems exist in the release, you must explicitly document them. Nothing causes more agony for a tester--or any other person for that matter--than spending hours identifying a bug only to be told later by the development group, "Oh yeah, we already know about that one." Not only does this result in an untold waste of time and effort, but it is grounds for murder.
VIII. Notes

This section contains any general information that may help the tester understand the product. If a developer finds information that is helpful or necessary, but doesn't fit neatly into any of the other categories, this is the place to put it. This paragraph may contain information a diverse as special schedule considerations and personnel.
IX. Appendices

This section contains any supporting material such as templates or other documents necessary or helpful to understanding the release.
X. Sign Off

Finally, to put the bite in the VDD, a variety of individuals must sign off on it. The required signatures will vary based on the organization and the environment. Depending on how structured the environment is, project leaders and managers may be the logical people to authorize the VDD. Again, because the needs of each organization are different, this issue needs to be taken up at various levels.
Make It a Habit

Implementing a VDD procedure will not cure all the testing and maintenance problems of an organization. However, it is a good, structured method that provides great benefit. Most members of a product group, whether testers, developers, or users, will agree that these benefits exist. Getting into the habit of using a VDD may take a little getting used to, but in the long run, a VDD can improve the quality of a software product at little cost.
</pre>
