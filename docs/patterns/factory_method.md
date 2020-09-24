# Pattern: Factory Method

Define an interface for creating an object, but let subclasses decide which class to instantiate. Factory Method lets a class defer instantiation to subclasses.
Defining a "virtual" constructor.
The new operator considered harmful.

* **Classification:** Creational

Gamma, Erich. Design Patterns: Elements of Reusable Object-Oriented Software (Addison-Wesley Professional Computing Series), pg 107

## Diagrams

* Include / link to visual representations of this pattern

## Context

* How does this pattern help to complete larger patterns?
* What is the intent of this pattern?

## Problem

```
TODO: Break up
```

A framework needs to standardize the architectural model for a range of applications, but allow for individual applications to define their own domain objects and provide for their instantiation.

Factory Method is to creating objects as Template Method is to implementing an algorithm. A superclass specifies all standard and generic behavior (using pure virtual "placeholders" for creation steps), and then delegates the creation details to subclasses that are supplied by the client.

Factory Method makes a design more customizable and only a little more complicated. Other design patterns require new classes, whereas Factory Method only requires a new operation.

People often use Factory Method as the standard way to create objects; but it isn't necessary if: the class that's instantiated never changes, or instantiation takes place in an operation that subclasses can easily override (such as an initialization operation).

Factory Method is similar to Abstract Factory but without the emphasis on families.

Factory Methods are routinely specified by an architectural framework, and then implemented by the user of the framework.

## Solution

The implementation of Factory Method discussed in the Gang of Four (below) largely overlaps with that of Abstract Factory. For that reason, the presentation in this chapter focuses on the approach that has become popular since.

An increasingly popular definition of factory method is: a static method of a class that returns an object of that class' type. But unlike a constructor, the actual object it returns might be an instance of a subclass. Unlike a constructor, an existing object might be reused, instead of a new object created. Unlike a constructor, factory methods can have different and more descriptive names (e.g., Color.make_RGB_color(float red, float green, float blue) and Color.make_HSB_color(float hue, float saturation, float brightness)

The client is totally decoupled from the implementation details of derived classes. Polymorphic creation is now possible.

Checklist:
If you have an inheritance hierarchy that exercises polymorphism, consider adding a polymorphic creation capability by defining a static factory method in the base class.
Design the arguments to the factory method. What qualities or characteristics are necessary and sufficient to identify the correct derived class to instantiate?
Consider designing an internal "object pool" that will allow objects to be reused instead of created from scratch.
Consider making all constructors private or protected.


## Consequences

* What are the benefits of using this pattern?
* What are the liabilities with using this pattern?

## Known Uses

* Describe known applications for this pattern
* Link to uses of the pattern within the source code

## Variants

Link to any patterns that are varients of this

## Related Patterns

Abstract Factory classes are often implemented with Factory Methods, but they can be implemented using Prototype.
Factory Methods are usually called within Template Methods.
Factory Method: creation through inheritance. Prototype: creation through delegation.
Often, designs start out using Factory Method (less complicated, more customizable, subclasses proliferate) and evolve toward Abstract Factory, Prototype, or Builder (more flexible, more complex) as the designer discovers where more flexibility is needed.
Prototype doesn't require subclassing, but it does require an Initialize operation. Factory Method requires subclassing, but doesn't require Initialize.
The advantage of a Factory Method is that it can return the same instance multiple times, or can return a subclass rather than an object of that exact type.
Some Factory Method advocates recommend that as a matter of language design (or failing that, as a matter of style) absolutely all constructors should be private or protected. It's no one else's business whether a class manufactures a new object or recycles an old one.
The new operator considered harmful. There is a difference between requesting an object and creating one. The new operator always creates an object, and fails to encapsulate object creation. A Factory Method enforces that encapsulation, and allows an object to be requested without inextricable coupling to the act of creation.

## Notes

Letting aside whether or not it is a good idea to have constructors do different things provided the same data, there is a much simpler way to solve the problem. We can simply give a name to our different constructors by making them functions instead, a design pattern known as Factory method:

```
class MyClass
{
public:
static MyClass constructThisWay();
static MyClass constructThatWay();
// ...
};
auto x = MyClass::constructThatWay();
```

The effect is globally the same – we have a way to select at compile time how we create an instance of MyClass – but it delivers the results in a much simpler and idiomatic way.
In terms of performance, RVO/NRVO should elide the copies.




If you compare both codes, you can see that using a factory:
* Reduces the number of line of code.
* Avoid code duplication.
* Organise the code: the factory has the reponsability to build a CarComparator and the business class just uses it.
The last point is important (in fact, they’re all important!) because a it’s about separation of concerns. A business class shouldn’t have to know how to build a complex object it needs to use: the business class needs to focus only on business concerns. Moreover, it also increases the division of work among the developers of the same project:
* One works on the CarComparator and the way it’s created.
* Others work on business objects that use the CarComparator.


This could be useful for creating drivers from chip peripherals? or other multi-constructor cases

Disambiguation

Let’s assume that you have a class with multiple constructors (with different behaviors). How can you be sure that you won’t use the wrong constructor by mistake?
Let’s look at the following code:

```
 class Example{
     //constructor one
     public Example(double a, float b) {
         //...
     }
    //constructor two
     public Example(double a) {
         //...
     }
     //constructor three
     public Example(float a, double b) {
         //...
     }
}
```

Though constructor one and two doesn’t have the same number of arguments, you can quicky fail to choose the right one, especially at the end of a busy day using the nice autocomplete from you favorite IDE (I’ve been there). It’s even more difficult to see the difference between constructor one and constructor three. This example looks like a fake one but I saw it on legacy code (true story !).
The question is, how could you implement different constructors with the same type of parameters (while avoiding a dirty way like the contructors one and three) ?
Here is a clean solution using a factory:

 class Complex {
     public static Complex fromCartesian(double real, double imag) {
         return new Complex(real, imag);
     }

     public static Complex fromPolar(double rho, double theta) {
         return new Complex(rho * Math.cos(theta), rho * Math.sin(theta));
     }

     private Complex(double a, double b) {
         //...
     }
 }

In this example, using a factory adds a description of what the creation is about with the factory method name:  you can create a Complex number from cartesian coordinates or from polar coordinates. In both cases, you know exactly what the creation is about.

Loose coupling

Another advantage of factories is the loose coupling.
Let’s assume you write a program that computes stuff and needs to write logs. Since it’s a big projet, one of your mate codes the class that writes the logs into a filesystem (the class FileSystemLogger) while you’re coding the business classes. Without factories, you need to instanciate the FileSystemLogger with a constructor before using it:

```
public class FileSystemLogger {
   ...
   public void writeLog(String s) {
   //Implemation
   }
}
...
public void someFunctionInClassXXX(some parameters){
   FileSystemLogger logger= new FileSystemLogger(some paramters);
   logger.writeLog("This is a log");
}
```

But what happens if there is a sudden change and you now need to write logs in a database with the implememtation DatabaseLogger? Without factories, you’ll have to modify all the functions using the FileSystemLogger class. Since this logger is used everywhere you’ll need to modify hundreds of functions/classes whereas using a factory you could  easiliy switch from one implementation to another by modifying only the factory:

```
//this is an abstraction of a Logger
public interface ILogger {
   public void writeLog(String s);
}

public class FileSystemLogger implements ILogger {
   ...
   public void writeLog(String s) {
      //Implemation
   }
}

public class DatabaseLogger implements ILogger {
   ...
   public void writeLog(String s) {
      //Implemation
   }
}

public class FactoryLogger {
   public static ILogger createLogger() {
      //you can choose the logger you want
      // as long as it's an ILogger
      return new FileSystemLogger();
   }
}
////////////////////some code using the factory
public class SomeClass {
   public void someFunction() {
      //if the logger implementation changes
      //you have nothing to change in this code
      ILogger logger = FactoryLogger.createLogger();
      logger.writeLog("This is a log");
   }
}
```

If you look at this code, you can easily change the logger implementation from FileSystemLogger to DatabaseLogger. You just have to modify the function createLogger() (which is a factory). This change is invisible for the client (business) code since the client code use an interface of logger (ILogger) and the choice of the logger implementation is made by the factory. By doing so, you’re creating a loose coupling between the implementation of the logger and the parts of codes that uses the logger.


factory method pattern

The factory method pattern is a more abstract factory. Here is the definition of the pattern given by the “Gang of Four”:
“Define an interface for creating an object, but let subclasses decide which class to instantiate. Factory Method lets a class defer instantiation to subclasses”
Here is a simplified UML diagram of the factory method pattern:

This diagram looks like the simple factory one (in its non-static form). The only (and BIG !) difference is the interface Factory:
* the Factory represents the “interface for creating an object”. It describes a factory method: getObjects().
* the ConcreteFactory represents one of the “subclasses [that] decide which class to instantiate”. Each ConcreteFactory has its own implementation of the factory method getObjects().
In the diagram getObjects() has to return a Type (or its subtypes). Which means that one conctrete factory could return a Subtype1 whereas another could return a SubType2.
Why using a factory method pattern instead of a simple factory?
Only when your code requires multiple factory implementations. This will force each factory implemantion to have the same logic so that a developer that uses one implementation can easily switch to another one without wandering how to use it (since he just have to call the factory method that has the same signature).

Since this is abstract, let’s go back to the car example. It’s not a great example but I use it so that you can see the difference with the simple factory, (we’ll see the real examples to understand the power of this pattern):

```
/////////////////////////the products
public interface Car {
   public void drive();
}

public class Mustang implements Car{
   public void drive() {
      //        some stuff
   }
   ...
}

public class Ferrari implements Car{
   public void drive() {
      //        some stuff
   }
        ...
}
///////////////////////// the factory
//the definition of the interface
public interface CarFactory{
  public Car getCar() {}
}

//the real factory with an implementation of the getCar() factory method
public class ConcreteCarFactory implements CarFactory{
   //this class is instantiable
   public CarFactory(){
      //some stuff
   }
   public Car getCar() {
      //        choose which car you want
      return new Ferrari();
   }
}
...
/////////////////////////some code using the factory
public static void someFunctionInTheCode(){
   CarFactory carFactory = new ConcreteCarFactory();
   Car myCar = carFactory.getCar();
   myCar.drive();
}

```

If you compare this code with the simple factory, I added this time an interface (CarFactory). The real factory (ConcreteCarFactory) implements this interface.
As I said this is not a great example because in this example you shouldn’t use a factory method pattern since there is only one concrete factory. It would be useful only if I have multiple implementations like SportCarFactory, VintageCarFactory, LuxeCarFactory, CheapCarFactory … . In this case, a developper could easily switch from one implementation to another since the factory method is always getCar().
