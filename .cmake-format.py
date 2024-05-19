# ----------------------------------
# Options affecting listfile parsing
# ----------------------------------
#with section("parse"):

  ## Specify structure for custom cmake functions
  #additional_commands = { 'foo': { 'flags': ['BAR', 'BAZ'],
  #           'kwargs': {'DEPENDS': '*', 'HEADERS': '*', 'SOURCES': '*'}}}

  ## Override configurations per-command where available
  #override_spec = {}

  ## Specify variable tags.
  #vartags = []

  ## Specify property tags.
  #proptags = []

# -----------------------------
# Options affecting formatting.
# -----------------------------
with section("format"):

  # If an argument group contains more than this many sub-groups (parg or kwarg
  # groups) then force it to a vertical layout.
  max_subgroups_hwrap = 2

  # If a positional argument group contains more than this many arguments, then
  # force it to a vertical layout.
  max_pargs_hwrap = 2

  # If a cmdline positional group consumes more than this many lines without
  # nesting, then invalidate the layout (and nest)
  max_rows_cmdline = 2

  # Format keywords consistently as 'lower' or 'upper' case
  keyword_case = 'upper'

# ------------------------------------------------
# Options affecting comment reflow and formatting.
# ------------------------------------------------
with section("markup"):

  # If comment markup is enabled, don't reflow the first comment block in each
  # listfile. Use this to preserve formatting of your copyright/license
  # statements.
  first_comment_is_literal = True

