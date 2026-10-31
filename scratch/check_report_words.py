import re
import sys

def analyze_markdown(path):
    with open(path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Find subsections
    subsections = re.findall(r'(##\s+(\d+\.\d+)\s+([^\n]+)\n(.*?))(?=\n##\s+\d+\.\d+|\n#\s+\d+\.|\Z)', content, re.DOTALL)
    
    limits = {
        '1.1': 250, '1.2': 250, '1.3': 250, '1.4': 250,
        '2.1': 400, '2.2': 400, '2.3': 400,
        '3.1': 400, '3.2': 400, '3.3': 400,
        '4.1': 400, '4.2': 400, '4.3': 400,
        '5.1': 400, '5.2': 400,
        '6.1': 400, '6.2': 400, '6.3': 400,
        '7.1': 250, '7.2': 250, '7.3': 250, '7.4': 200,
        '8.1': 400
    }

    print(f"Found {len(subsections)} subsections.")
    for full_match, num, title, body in subsections:
        # Strip code blocks, image links, table pipes, markdown links syntax for word count
        clean_text = re.sub(r'```.*?```', '', body, flags=re.DOTALL)
        clean_text = re.sub(r'!\[.*?\]\(.*?\)', '', clean_text)
        clean_text = re.sub(r'\[(.*?)\]\(.*?\)', r'\1', clean_text)
        clean_text = re.sub(r'\|', ' ', clean_text)
        clean_text = re.sub(r'[-*#_`$]', ' ', clean_text)
        
        words = clean_text.split()
        count = len(words)
        limit = limits.get(num, None)
        status = "OK"
        if limit:
            if count > limit:
                status = f"EXCEEDED ({count} > {limit})"
            else:
                status = f"PASSED ({count} / {limit} words)"
        print(f"Section {num} - {title.strip()}: {status}")

if __name__ == '__main__':
    analyze_markdown('REPORT.md')

