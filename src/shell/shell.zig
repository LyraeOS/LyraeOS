const std = @import("std");
const c = @cImport({
    @cInclude("screen/tty.h");
    @cInclude("shell/gol.h");
    @cInclude("shell/minesweeper.h");
    @cInclude("shell/matrix.h");
});
pub fn kprint(comptime fmt: []const u8, args: anytype) !void {
    var buf: [1024]u8 = std.mem.zeroes([1024]u8);
    const bytes = try std.fmt.bufPrint(buf[0..buf.len], fmt, args);
    for (bytes) |b| {
        c.kputchar(b);
    }
}
const LineEditor = struct {
    buffer: []u8,
    len: usize,
    const InputAction = enum {
        none,
        submit,
        backspace,
    };

    pub fn init(buf: []u8) LineEditor {
        return .{
            .buffer = buf,
            .len = 0,
        };
    }
    fn push(self: *LineEditor, ch: u8) bool {
        if (self.len >= self.buffer.len - 1) return false;
        self.buffer[self.len] = ch;
        self.len += 1;
        return true;
    }
    fn pop(self: *LineEditor) bool {
        if (self.len == 0) return false;
        self.len -= 1;
        self.buffer[self.len] = 0;
        return true;
    }
    pub fn empty(self: *LineEditor) void {
        self.len = 0;
        @memset(self.buffer[0..], 0);
    }
    pub fn slice(self: *LineEditor) []const u8 {
        return self.buffer[0..self.len];
    }
    pub fn handle_char(self: *LineEditor, ch: u8) InputAction {
        return switch (ch) {
            '\n' => .submit,
            '\x08' => if (self.pop()) .backspace else .none,
            else => if (self.push(ch)) .none else .none,
        };
    }
};
const CommandInfo = struct { name: []const u8, description: []const u8 };
const commands: []const CommandInfo = &[_]CommandInfo{ .{
    .name = "echo",
    .description = "prints args given",
}, .{
    .name = "gol",
    .description = "conways game of life\n   gol [cell size > 2]",
}, .{
    .name = "clear",
    .description = "clears the screen",
}, .{
    .name = "panic",
    .description = "gives a test panic message",
}, .{
    .name = "mine",
    .description = "minesweeper",
}, .{
    .name = "help",
    .description = "prints this menu",
}, .{
    .name = "matrix",
    .description = "matrix style ui",
} };
fn command_handler(line: []const u8) !void {
    var it = std.mem.tokenizeAny(u8, line, " \t");
    var args_b: [16][]const u8 = undefined;
    var argc: usize = 0;
    while (it.next()) |tok| : (argc += 1) {
        if (argc >= args_b.len) break;
        args_b[argc] = tok;
    }
    const cmd = args_b[0];
    const args = args_b[1..argc];
    c.tty_set_cursor_enabled(0);
    if (std.mem.eql(u8, cmd, "echo")) {
        for (args) |arg| {
            try kprint("{s} ", .{arg});
        }
        c.kputchar('\n');
    } else if (std.mem.eql(u8, cmd, "clear")) {
        c.tty_clear();
    } else if (std.mem.eql(u8, cmd, "gol")) {
        if (argc < 2) {
            try kprint("Usage:\n    gol [cell_size > 2]\n", .{});
        } else {
            _ = std.fmt.parseInt(i32, args[1], 10) catch 2;
            c.ConwaysMain(2);
            c.tty_clear();
        }
    } else if (std.mem.eql(u8, cmd, "panic")) {
        c.panic("uh oh :(");
    } else if (std.mem.eql(u8, cmd, "mine")) {
        c.minesweeperMain();
    } else if (std.mem.eql(u8, cmd, "matrix")) {
        c.matrix();
        c.tty_clear();
    } else if (std.mem.eql(u8, cmd, "help")) {
        try kprint("Available commands:\n", .{});
        for (commands) |command| {
            try kprint("{s}:\n   {s}\n", .{ command.name, command.description });
        }
    } else {
        try kprint("Unknown command: {s}\n", .{cmd});
    }
    c.tty_set_cursor_enabled(1);
}
export fn shell_loop() noreturn {
    c.tty_clear();
    var buf: [64]u8 = undefined;
    var le = LineEditor.init(buf[0..]);
    _ = c.keyboard_pop(&c.keypress_queue);
    kprint("Welcome to LyraeOS!\n", .{}) catch {};
    kprint("kernel@lyraeos $ ", .{}) catch {};
    while (true) {
        const char: u8 = c.wait_for_key(&c.keypress_queue);
        switch (le.handle_char(char)) {
            .backspace => c.tty_backspace(),
            .none => if (char >= 32) c.kputchar(char),
            .submit => {
                c.kputchar('\n');
                command_handler(le.slice()) catch {};
                le.empty();
                kprint("kernel@lyraeos $ ", .{}) catch {};
            },
        }
    }
}
