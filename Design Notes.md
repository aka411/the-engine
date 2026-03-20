



# Type safe Handles

Currently handles are just made with "using HandleType = uint32_t" this is not type safe and need another way to make handles type safe.



# who informs renderer of view port size change?
1) i could let game inform renderer by letting them relay it.
2) or have a internal method run to check it and update. cons -> runs every frame check
3) give a call back from rendering system to input or window system.like suscriber listner