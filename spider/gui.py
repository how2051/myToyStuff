import tkinter as tk
from tkinter.scrolledtext import ScrolledText

def load_data():
    try:
        with open("news_output.txt", "r", encoding="utf-8") as f:
            text_widget.delete(1.0, tk.END)
            text_widget.insert(tk.END, f.read())
    except FileNotFoundError:
        text_widget.delete(1.0, tk.END)
        text_widget.insert(tk.END, "can't find file!")


def clear_data():
    text_widget.delete(1.0, tk.END)
    try:
        with open("news_output.txt", "w", encoding="utf-8") as f:
            f.write("")
    except Exception as e:
        text_widget.insert(tk.END, f"\nclear failed! : {e}")


root = tk.Tk()
root.title("Newspaper")


root.configure(bg="#2E3440")

text_widget = ScrolledText(
    root, wrap=tk.WORD,
    width=100, height=10,
    bg="#2E3440", fg="white",
    insertbackground="white"
)
text_widget.pack(padx=10, pady=10, fill=tk.BOTH, expand=True)

root.update_idletasks()
min_width = text_widget.winfo_reqwidth() + 40
min_height = text_widget.winfo_reqheight() + 65
root.minsize(min_width, min_height)

button_frame = tk.Frame(root, bg="#2E3440")
button_frame.pack(pady=10)

# refresh_button = tk.Button(root, text="refresh", command=load_data, bg="#2E3440", fg="white")
# refresh_button.pack(pady=10)
# refresh_button = tk.Button(root, text="clear", command=clear_data, bg="#2E3440", fg="white")
# refresh_button.pack(pady=10)

refresh_button = tk.Button(
    button_frame,
    text="Refresh",
    command=load_data,
    bg="#2E3440",
    fg="white",
    # font=("Arial", 10, "bold")
)
refresh_button.pack(side=tk.LEFT, padx=5)

clear_button = tk.Button(
    button_frame,
    text="Clear",
    command=clear_data,
    bg="#2E3440",
    fg="white",
    # font=("Arial", 10, "bold")
)
clear_button.pack(side=tk.LEFT, padx=5)


load_data()


root.mainloop()
