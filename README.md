# UE FPS Item Interaction System Demo
 A simple demo built using Unreal Engine. The item grabbing system has a C++ base meant to be easily extended in blueprints. See linked video below.
 
## To Use On Your Own Project
Before opening the blueprint files in UE5 or compiling the code files in the project, remember to change the macro at the top of each class to the name of the target project
 
 Example:
 ```
UCLASS()
class YOURPROJECTNAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
 ```
 
 Additionally to get the blueprint to work out of the box, you must add engine redirects to point to the classes refactored for your project.
 To do this open up the *DefaultEngine.ini* file in the *Scripts* folder of your UE project and add the redirects under the [/Script/Engine.Engine] line]
 Remember to omit UE labels for inherited classes (PlayerCharacter instead of APlayerCharacter)
 
 Example:
 ```
 [/Script/Engine.Engine] line]
 +ActiveClassRedirects=(OldClassName="PlayerCharacter",NewClassName="/Script/**YourProjectName**.PlayerCharacter")
 ```
 
Once you have changed the macro and added the redirects, compile the project and you should be able to use it without issue, including the blueprints.

## Overview

The *InteractiveItemBase* class contains events that are triggered by the *PlayerCharacter* class at different points of player interaction. The player can pick up items, rotate them and put them back down, triggering the events which can be defined in blueprints to create different behaviours between items. Additionaly there is some UI that allows each item to have one or more "Item Actions" which can also be defined in blueprints and might involve interactions other than rotating it.

In the video example below you can observe a cube which prints messages to the screen when these different events are triggered, then a bottle which has no functionality other than rotating. There are also two different variations of a letter, one that plays an open animation when a button is pressed and can be read whilst holding the letter upright and opposite facing to the player and another letter which opens manually by dragging the mouse and closes up when being put down unlike the first. All items are timestamped on the video description.

[UE5 FPS Item Interaction System Demo](http://www.youtube.com/watch?v=Vb69OTXMb1Y)

![IMAGE ALT TEXT](http://img.youtube.com/vi/Vb69OTXMb1Y/0.jpg)
