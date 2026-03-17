---
layout: home
title: FPGA VGA Driver Project
tags: fpga vga verilog
categories: demo
---
# **WELCOME TO MY FPGA VGA DRIVER PROJECT**
Hi my name is Seán Maloney and this is my VGA driver Project, For my project I did a Irish flag that was made using a modified version of Colour Stripes provided by my teacher. 
## **Template VGA Design**
### **Project Set-Up**
The template design was pretty straightforward demo for how to implement VGA signal generation. It displayed vertical stripes of colors similar to old VGA screens that produced that image when no picture was being provided to them, this reallt helped me understand the fundementals of how I was going to attempt VGA timing and signal synchronization.

I set up the project in Vivado, and I ensured all the design files were correctly integrated and ready for the simulation, synthesis and implementation. Vivado's enviorment gave me all the tools I needed for checking the design flow, resource usage, and timing requirements. This was important because I needed to see how the different parts of the FPGA workflow and how they connect to ensure the design could run on the Basys 3 board provided to me.

Below is a screenshot of my project summary, showing the inital setup and design flow:

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/Project overview.png">

This template helped me see how the VGA timing is used and handled and also how the color are displayed on the screen

### **Template Code**
The template's key modules were:
- **VGASync.v:** This module generates the horizontal and vertical sync signals (hsync and vsync), this tracks the current position of the pizels on the screen, we can then ensure the display is being updated row by row and frame by frame by using (hcount and vcount).
- **VGAColourStripes.v:** This module controls the colors outputted based on the position of the pixels. The default design was to divide the screen into multiple vertical stripes, each with a different color.

For example part of the original code was assiging colors like this:
`if (col >= 0 && col < 210) begin`
    `red_next <= 4'b1111;   // Red stripe`
    `green_next <= 4'b0000;`
    `blue_next <= 4'b0000;`
`end else if (col >= 220 && col < 430) begin`
    `red_next <= 4'b0000;   // Green stripe`
    `green_next <= 4'b1111;`
    `blue_next <= 4'b0000;`
`end`
This logic was simple yet effective, splitting the screen into multiple vertical stripes. This really helped me understand the relationship between pixel co-ordinates and screen output.

The template has a modular structure while made it way easier to adapt and expand the code for my own design ideas.

### **Simulation**
The simulation process was key to my understanding of how the design works. I used Vivado's built in simulator to check the hsync and vsync, and color signals over time. The waveform output showed the exact timing of the sync pulses, this ensured the design was adhering to VGA's specifications.

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/init sim.png">

For the template design, the simulation had to verify the color regoins were mapped correctly to specific sections of the screen. The timing diagram shows the tranistions between multiple color zones, this confirms the counters were incrementing as they were specified to.

### **Synthesis & Implementation**
The synthesis process for the initial template design translated the code into a netlist which could then be supplied onto the FPGA hardware. Vivado's synthesis tool provided me some detailed and informative insights into the design's resource usage, logic mappings and the timing estimates. These reports also confiremed that the template design had be utilized with minimal logic and memory which was well withing the capabilities of the Basys 3 board Artix 7. This meant that it indicated the basic design that my teacher had provided was straightforward combinational logic with simple counters and was efficient and ready for the FPGA programming

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/InitSynReport.png">

After synthesis, the implementation was the next step and it succesfully programmed the design onto the FPGA hardware. This invloved mapping the generated bitstream to the board. from there we can connect the FPGA to a VGA monitor. When succesfully connected the monitor begins to display colorful stripes as it was intended. This successfully demonstrated the design's functionality and how smooth the synchronization of the VGA timing signals is. The process shows the effectiveness of the modular design and its readiness for real world application.

### **Demonstration**
Finally, I programmed the FPGA and connected it to a VGA monitor. The screen displayed colorful Stripes that were vertical. It was amazing to see some image actually displaying to a monitor.

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/ColorStripes.JPG">

## **My VGA Design Edit**
### **Code Adaptation**
After understanding the templates, I decided to give it a go and try to display an irish flag. This involved adjusting the color regions in the `VGAColourStripes.v` module. 

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/Irish flag.jpg">


This image isn't that complex but takes an understanding of how the image fits on the screen, It can obtained by expermenting with the code until you understand and start seeing a desireable image as a result.


### **Simulation**
I simulated the Irish flag design to confirm the correct color assignments and transitions between the sections. The waveform that was produced by this showed that the hcount values aligned perfectly with the boundaries of each stripe on the Irish flag, the colors also matched what was expected (Grenn, White, and Orange)

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/irish sim.png">

Simulation also helpes me debug an issue where the colors were overlapping due to incorrect boundary values. After adjusting the values within the `if` statments, the simulation output looked perfect.

### **Synthesis**
The synthesis process converted my Verilog coe into a netlist format, this was suitable for the FPGA hardware. Vivado's synthesis tool reported no signicant increase in the resource usage compared to the template design I initially had, this is because they both primarily relied on simple combinational logic for the assignmend of colors and counters for the pixel tracking. Key metrics like logic cell usage and timing estimates were also within the acceptable limits for the board. This ensured the design could be implemented efficiently on the board.

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/Report Irish.png">

In addition to this, the synthesis also provided helpful insights into what optimaztions could be implemented like logic to reduce delays and also improve the timing performance. Vivado also suggested areas for isnpection and refinement, like adding some timing constraints for the pixel clock. This process not only ensured my hardware was compatiable but it also gave me a lesson on why we need and should value synthesis as this is the critical step for refining designs before going to implementation

### **Demonstration**
After programming the board, the Irish flag appeared on the VGA monitor, with the Three Vertical stripes of Green white and orange seperated by thinner black vertical stripes to make the picture more detailed. This was a great success and a satisfying moment as it showed my hard work payed off.

<img src="https://raw.githubusercontent.com/sean-maloney/SOCBlog/main/docs/assets/images/Irish flag.jpg">

## **Failed Experiments**
In my attempt to add more creativity and complexity into my code, I tried animatig the flag by shifting shifting the stripes horizontially. However, the timing logic did not work as I expected or hoped it would. I also attempted a slideshow feature that would use a `switch-case` statement to display different flags but I couldn't manage the state transitions.

These failed attempts helped me to learn about the handling of timing-sensitive designs and debugging complex Verilog logic.

## **Reflections**
This project has been a great introduction to FPGA deisgn and Verilog. The templates were a great help and helped me understand and learn the basics, and when I modified the templates to display the Irish flag, that only deepend my understanding of VGA timing and color logic. Even though some of my experiments didn't go the way I planned them to go, they gave me ideas for my future projects.

I'm excited to try and learn some more complex designs someday, or even try and learn some animation or interactive features to implement into my future projects
