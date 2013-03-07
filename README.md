# Hap

**Hap** is a dynamically typed, asynchronous, imperative programming language.

Here is a simple example of Hap&rsquo;s syntax and semantics:

```
var health = 100;

when (health <= 0) {
  print("Goodbye, cruel world!");
  exit();
}

print("Hello, sweet world!");

while (true)
  --health;
```

Hap code reads much like code in other imperative languages. Its syntax is reminiscent of JavaScript. However, note the use of `when`: this is an *asynchronous conditional*. The above code prints:

```
Hello, sweet world!
Goodbye, cruel world!
```

*Synchronous* flow control statements such as `if` and `while` are evaluated immediately. If the condition is true *when the statement is reached*, then the body is evaluated immediately, before proceeding to subsequent statements.

*Asynchronous* flow control statements such as `when` are evaluated concurrently. The flow of control proceeds to subsequent statements immediately; only when the conditional actually becomes true is the body evaluated.

This allows programs to be structured implicitly in terms of events and time-based constraints. You are not forced to subscribe to event handlers for predefined conditions&mdash;rather, you can create ad-hoc conditions as needed, and this is syntactically lightweight.

# Types

<table>
<tr><th>Type</th><th>Examples</th></tr>
<tr>
<td><code>int</code></td>
<td><pre><code>0
1234</code></pre></td>
</tr>
<tr>
<td><code>bool</code></td>
<td><pre><code>false
true</code></pre></td>
</tr>
<tr>
<td><code>text</code></td>
<td><pre><code>""
"scare"</code></pre></td>
</tr>
<tr>
<td><code>list</code></td>
<td><pre><code>[]

[1, 2, 3, 4, 5]

["this", "that", "and", "the", "other", "thing"]

[ [ +cos(x), -sin(x) ],
  [ +sin(x), +cos(x) ] ]

[0, false, ""]</code></pre></td>
</tr>
<tr>
<td><code>map</code></td>
<td><pre><code>{}

{ "one": 1  "two": 2  "three": 3 }

[ { en: "one",   fr: "un"    },
  { en: "two",   fr: "deux"  },
  { en: "three", fr: "trois" } ]</code></pre></td>
</tr>
</table>

# Statements

<table>
<tr><th>Statement</th><th>Description</th></tr>
<tr>
<td><pre><code>var NAME;
var NAME = EXPR;</code></pre></td>
<td>Creates a lexically scoped variable named <code>NAME</code>. Initializes it to <code>EXPR</code>, or gives it an initial value of <code>undef</code> if <code>EXPR</code> is not specified.</td>
</tr>
<tr>
<td><code>if EXPR STAT</code></td>
<td>Synchronous. Executes <code>STAT</code> if <code>EXPR</code> is true when the statement is reached.</td>
</tr>
<tr>
<td><code>when EXPR STAT</code></td>
<td>Asynchronous. Executes <code>STAT</code> once the first time <code>EXPR</code> becomes true. If <code>EXPR</code> is true when the statement is reached, executes <code>STAT</code> immediately.</td>
</tr>
<tr>
<td><code>whenever EXPR STAT</code></td>
<td>Asynchronous. Executes <code>STAT</code> once every time <code>EXPR</code> becomes true. If <code>EXPR</code> is true when the statement is reached, executes <code>STAT</code> immediately.</td>
</tr>
<tr>
<td><code>while EXPR STAT</code></td>
<td>Synchronous. Executes <code>STAT</code> repeatedly as long as <code>EXPR</code> remains true. If <code>EXPR</code> is false when the statement is reached, does nothing.</td>
</tr>
<tr>
<td><code>repeat_when EXPR STAT</code></td>
<td>Asynchronous. The first time <code>EXPR</code> becomes true, executes <code>STAT</code> repeatedly. Stops when <code>EXPR</code> becomes false.</td>
</tr>
<tr>
<td><code>repeat_whenever EXPR STAT</code></td>
<td>Asynchronous. Every time <code>EXPR</code> becomes true, executes <code>STAT</code> repeatedly. Pauses whenever <code>EXPR</code> becomes false and resumes whenever it becomes true.</td>
</tr>
<tr>
<td><code>EXPR;</code></td>
<td>Synchronous. Evaluates <code>EXPR</code> and discards the result.</td>
</tr>
<tr>
<td><code>{ STAT... }</code></td>
<td>Synchronous. Evaluates a block of statements as a unit. Introduces a new lexical scope.</td>
</tr>
</table>
