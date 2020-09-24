#ifndef COMMON_H_
#define COMMON_H_

/// To use:
/// CONSTANT(size, 5);
/// char buffer[size];
#define CONSTANT(name, value) \
	enum                      \
	{                         \
		name = value          \
	}

#endif // COMMON_H_
