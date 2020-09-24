# Value Proposition

Embedded Framework value proposition:

1. Spend less time focused on low-level firmware details and more time focusing on your application logic
2. We provide a customized, working solution to your company
	* You do not need to take the time to customize the framework to your board
3. We provide support
	* Many vendor SDKs don't or also require you to pay
	* If your part or chip doesn't have a driver, we can implement the code for you
4. Our framework decouples the application logic from the underlying hardware and OS
	* enabling better portability
	* faster iteration on hardware changes
	* enabling ability to test competing hardware designs with the same program (e.g., evaluating different parts for performance)
5. We can bring up our framework on your board much faster than you can bring up your board on your own
6. We make key architecture decisions for you, helping ensure that you do not make common mistakes in your design
	* Our archicture remains flexible - your team can still use their preferred design model
7. We will reduce your schedule and prevent delays:
	* Hard to hire firmware engineers, you can now focus on library & application-level developers
	* Write and test software in parallel with hardware designs, enabling faster bring-up and iteration
	* Your developer won't spend 3 months recreating the basic utilities & classes needed for embedded development
8. Improved developer workflows
	* Easier to develop, test, and iterate on a PC
	* Standard interfaces allow developers to write a driver or test logic on their PC, then port to the hardware with the confidence that it will work
9. Catch problems early in the design cycle
	* Built-in unit testing support
	* Static analysis and complexity metrics identify problem areas of the code early on
10. We can provide a custom CI server solution alongside the framework, ensuring that your code is built nightly and problems are identified as soon as possible
	* No need to hire a build engineer to manage this stuff for you!

## Notes

From Saket @ Noon:

* What if you did have this framework model?
    * 1-2 people on firmware
    * Fpcus more energy on a rock-solid application layer and having more of the senior people work on connectivity and user facing issues
    * Have less people who are there needing to grind through a bring-up process
    * Would have steered people to more higher level tasks
