-- The generic use objects subsystem
--
-- Load it with: load_subsystem("use_object")
--
-- Most module authors wont load this module as it it automatically included
-- by other object using subsystems. One wants to include it only when they are creating
-- a completly new object using subsystem.

constant("use_object", {})

use_object.CANNOT_USE = settag({}, newtag())
