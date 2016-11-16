# Plasmacore
- v0.4.3
- November 13, 2016

## Requirements
- Mac with Xcode and iOS SDK.
- Plasmacore comes with an embedded copy of the Rogue language; you do not need to install Rogue separately.

## Supported Platforms

Currently the Plasmacore toolchain only runs on MacOS and only supports an iOS compile target.

Platform | Status | Notes
---------|--------|---------------------------------------------
iOS      | Alpha  | No sound or direct virtual keyboard support.

Other platforms will eventually be supported.


## Documentation and Resources

There is some Rogue documentation here: [https://github.com/AbePralle/Rogue/wiki](https://github.com/AbePralle/Rogue/wiki)

There is no Plasmacore documentation yet.  You can manually browse the `Libraries/Rogue/Plasmacore` files.

A sample Plasmacore game project is available here: [https://github.com/AbePralle/PlasmacoreDemos](https://github.com/AbePralle/PlasmacoreDemos)


## Bootstrap Command

To bootstrap a new Plasmacore-based project, open a Terminal in your new project folder and copy and paste the following command:

    curl -O https://raw.githubusercontent.com/AbePralle/Plasmacore/master/Bootstrap.mk && make -f Bootstrap.mk

The command will fetch a bootstrap makefile which in turn will `git clone` the latest Plasmacore repo in a temporary folder and copy all the files into the current folder.


## Starting a New Project

1.  Run the bootstrap command or manually clone the Plasmacore repo and copy everything except the `.git` folder into your project folder.
2.  At the command line run `make ios`.  The first build will take a while as intermediate files are compiled.
3.  Open `Platforms/iOS/iOS-Project.xcodeproj` in Xcode and run on the simulator or a device.  You should see a blue screen.
4.  Edit `Source/Main.rogue` and add more game code.
5.  Either run `make ios` again or just compile and run in Xcode again as a build phase automatically runs `make ios`.  If you get an error in Xcode and you can't tell what it is, run `make ios` on the command line and you will see the compiler error message.
6.  Add images to Assets/Images and load them by name - `Image("img.png")`, `Font("SomeFont.png")`, etc.


## Updating an Existing Project

`make update` will update your current project to the latest version of Plasmacore (via `git` and `rsync`) without touching any game-specific files.


## License
Plasmacore is released into the Public Domain under the terms of the [Unlicense](http://unlicense.org/).


###v0.4.4 - November 15, 2016
- [iOS Project] An `Application.save` message now gets sent on `applicationWillResignActive` instead of `applicationDidEnterBackground` and `applicationWillTerminate`.
- [Rogue] Updated to Rogue v1.1.4

## Change Log

###v0.4.3 - November 13, 2016
- [Rogue] Fixed bug in `Value.to_json()` where `false` would print as `null`.

###v0.4.2 - November 13, 2016
- [Rogue] Fixed fatal compile error bug in 0.4.1 release.

###v0.4.1 - November 13, 2016
- [Rogue] Updated Rogue to v1.1.3.

###v0.4.0 - November 12, 2016
- [Plasmacore] Image compiler meta-info output format and corresponding Plasmacore image info loading system reworked for massive savings.  File sizes are reduced by half and runtime object counts are reduced by a factor of 10.
- [Box] Creating a `Box` from a `Value` with `Box(Value)` now works with value lists - `Box(@[3,4])` is equivalent to `Box(&size=XY(3,4))` and `Box(@[1,2,3,4])` is equivalent to `Box(&position=XY(1,2),&size=XY(3,4))`.
- [iOS] Now displays correctly on iPhone Plus - glViewport needs physical pixels (1080x1920) while the rest of the rendering system needs the internal display buffer size (1242x2208).

###v0.3.5 - November 7, 2016
- [App Lifecycle] Added State/View `on_app_start/stop/save` methods based on global `Application.start/stop/save` events.  Timers and such should be stopped and started with those methods.  `save()` is called when the app may or may not resume execution and should save its state.

###v0.3.4 - November 4, 2016
- [Image Compiler] Fixed bug where the same image being copied+resized more than once starts with the original image each time.

###v0.3.3 - November 3, 2016
- [Rogue] Updated to Rogue v1.1.2.

###v0.3.2 - November 1, 2016
- [Display] Added `Display.clipping_region:Box?` which sets up the "scissor test" that clips any drawn images to the given bounds.  Assign a valid box to define and enable the clipping region or `null` to disable it.
- [Image System] Removed kludgy `Plasmacore.default_asset_folder` in favor of a global property `Image.search_folders:String[]`.  Modify the folders as you like by adding e.g. `iPadRetina` to the `search_folders` list, allowing `Image("Name.png")` to be used rather than `Image("iPadRetina/Name.png")` etc.
- [Image System] Any path-shortened form of an image can now be used to load the image.  For example, `Image("iOS/BG.png")` can now be loaded as `Image("BG.png")`.

###v0.3.1 - October 31, 2016
- [Sound] Added `Sound.duration()->Real64` that returns the duration in seconds.
- [Sound] Added `Sound.is_finished()->Logical` that returns `true` if a sound is not playing and not paused.
- [Sound] Added `Sound.is_playing()->Logical`.
- [Sound] Added `Sound.is_repeating:Logical`.
- [Sound] Added `Sound.pause()` that stop a sound without rewinding it.  `play()` or `resume()` will play the sound again.
- [Sound] Added `Sound.play(is_repeating:Logical)` in addition to `Sound.play()`.
- [Sound] Added `Sound.position:Real64` that specifies a sound's playback position in seconds.
- [Sound] Added `Sound.resume()` that resumes playing a sound only if it has been previously paused.
- [Sound] Added `Sound.set_volume(Real64)` that accepts an argument between 0.0 and 1.0.
- [Sound] Added `Sound.stop()` that stops a sound and rewinds it to the beginning.
- [SoundGroup] Created new `SoundGroup` class to manage a set of related sounds, like multiple copies of a single sound effect or multiple variations of a sound.
- [SoundGroup] Added constructor `SoundGroup.init(sound_name:String,channels=1:Int32,&is_music)`.
- [SoundGroup] Added constructor `SoundGroup.init(&random,&random_order,&autoplay)`.  The `&random` flag picks a sound randomly each time.  `&random_order` is similar but it plays all sounds in a random order before repeating any of them, reshuffling each time before another pass.  `&autoplay` automatically plays another sound when each sound is finished - note `SoundGroup.play()` must be called once to start the process.
- [SoundGroup] Added `SoundGroup.add(Sound)` which adds another sound to the group.
- [SoundGroup] Added `SoundGroup.play()` that plays the next sound from the group.
- [SoundGroup] Added `SoundGroup.pause()` that pauses all sounds in the group.
- [SoundGroup] Added `SoundGroup.resume()` that resumes all paused sounds in the group.
- [SoundGroup] Added `SoundGroup.stop()` that halts playback of the group.

###v0.3.0 - October 31, 2016
- [Sound Compiler] Added new Sound Compiler that works similar to the Image Compiler.  It is automatically invoked during an Xcode build or you can manually `./scom iOS` to run it.  It uses the `Assets/Sounds/SoundConfig.txt` build script, creating a default version necessary.
- [Sound] Added `Sound` class.  Create a sound with `Sound(name:String,[channels=1:Int32])`.  `name` should be a filename in `Assets/Sounds/`; you can omit `Assets/Sounds/` as well as the extension.  `channels` is the number of times that sound can play concurrently - each additional number ends up creating a new sound player underneath so don't use more than 1 channel if you don't need to.
- [Sound] Call `Sound.play()` to play a sound.  Additional sound API calls will be added later.
- [AVFoundation Framework] You must manually add the `AVFoundation` framework to any existing projects.
- [Image Compiler] Renamed `imageCopy` command to `copy`.

###v0.2.3 - October 29, 2016
- [Image Compiler] Improved wildcard matching for `group` specifications.
- [Image Compiler] Changing `Assets/Images/ImageConfig.txt` no longer causes an image sheet recompile due to timestamp difference.  If you need to recompile existing images with new settings, do a `make clean` first.
- [TargetValue] Changed original initializer to default to `ProgressFn.QUADRATIC` instead of `LINEAR`.
- [TargetValue] Added new initializer `init(initial_value,duration_fn,progress_fn=ProgressFn.QUADRATIC)`.  `duration_fn` should be of type `Function($DataType)->(Real64)`, accepts a delta interval, and should return the desired duration for that interval.  The `TargetValue` will automatically adjust the duration as new target values are set, effectively allowing a rate-limited rather than time-based `TargetValue`.

###v0.2.2 - October 26, 2016
- [Image Compiler] Changed convention of Image Compiler to use `camelCase` commands with `serpent_case` arguments, like Rogue.
- [Image Compiler] Added `resize:WxH->WxH` option to Image Compiler's `imageCopy` command.
- [Image Compiler] Removed vestigial reference to 'freetype-2.6.3' folder in Image Compiler's Makefile.

###v0.2.1 - October 21, 2016
- [iOS Xcode Project] Added missing Swift files to iOS template project.

###v0.2.0 - October 20, 2016
- [Image Compiler] Replaced complex Asset Manager (`AM`) with simple Image Compiler (`ICOM`) that works off the script `Assets/Images/ImageConfig.txt`.  ICOM will automatically create a default config script if that is missing.  ICOM is automatically run as part of the `make ios` build.

###v0.1.2 - October 3, 2016
- [Bootstrap] Instead of copying all hidden files, now copies only `.gitattributes` and `.gitignore` and uses the `--ignore-existing` rsync option.
- [ActionCmd] Added `ActionCmd(Function())` constructor that creates an action command that calls the given function when invoked.
- [Image] `Image(existing:Image,subset:Box)` now works correctly even when the `existing` image hasn't been loaded yet.
- [Image] Renamed `alpha` to `opacity` (controls master opacity, defaults to `1.0`).
- [Image] Fixed `opacity` to work correctly with opaque images (was previously not enabling blending).
- [Font]  Renamed `alpha` to `opacity`; opacity now actually applied to font drawing.
- [TargetValue] Consolidated `TimeLimitedTargetValue` and `RateLimitedTargetValue` into single class `TargetValue`.
- [TargetValue] A `start_time` of 0 now signals a finished state internally; once target value is reached then `start_time` is set to 0 to avoid subsequent duration changes "restarting" animation.
- [XY] Added `operator==(XY)` to provide a better implementation than the current Rogue-generated default.

###v0.1.1 - October 3, 2016
- [Bootstrap] Added bootstrap mechanism detailed in README.

###v0.1.0 - October 2, 2016
- Initial release.
