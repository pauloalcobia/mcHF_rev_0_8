This is the latest revision of the mcHF firmware and hardware. I have decided
to slowly upload all files and documentation here, instead of a surprise release,
which was the way of doing things before. As this revision is much more complex
and riskier, it makes sense to take this approach.

The 0.8 mcHF revision builds on rev 0.7, in a way that it inherits the RF and Logic
boards hardware, but implements a new UI board that has its own application 
processor. This way the STM32 CPU on the Logic boards can be dedicated fully for
DSP tasks, while the extra CPU on the UI board can be utilized for user interface
handling.

So in simple terms, the UI driver on the Logic board firmware could be commented
out, and moved to the new application CPU.

There is still a lot of testing on the hardware left, as there are new ideas i am trying
to implement, but during the next 3-4 months i hope to be able to upload all
documentation and sources, as long as they become in presentable form.

73!