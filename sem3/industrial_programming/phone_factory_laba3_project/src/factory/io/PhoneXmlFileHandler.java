package factory.io;

import factory.model.Phone;
import factory.validation.PhoneValidator;
import org.w3c.dom.*;
import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class PhoneXmlFileHandler extends AbstractFileHandler<Phone> {

    @Override
    public List<Phone> readFromFile(String filename) {
        List<Phone> phones = new ArrayList<>();

        try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
            File file = new File(filename);
            if (!file.exists()) {
                String errorMsg = "XML file not found: " + filename;
                logWriter.println("Error: " + errorMsg);
                System.err.println("Error: " + errorMsg);
                return phones;
            }
            
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.parse(file);
            doc.getDocumentElement().normalize();
            NodeList nodeList = doc.getElementsByTagName("phone");

            for (int i = 0; i < nodeList.getLength(); i++) {
                Node node = nodeList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    Element element = (Element) node;
                    String idStr = getElementValue(element, "id");
                    String brand = getElementValue(element, "brand");
                    String model = getElementValue(element, "model");
                    String camerasStr = getElementValue(element, "cameraCount");
                    String dateStr = getElementValue(element, "releaseDate");
                    String priceStr = getElementValue(element, "price");

                    try {
                        if (!PhoneValidator.isValidId(idStr)) throw new IllegalArgumentException("Invalid ID");
                        if (!PhoneValidator.isValidBrand(brand)) throw new IllegalArgumentException("Invalid brand");
                        if (!PhoneValidator.isValidModel(model)) throw new IllegalArgumentException("Invalid model");
                        if (!PhoneValidator.isValidCameraCount(camerasStr)) throw new IllegalArgumentException("Invalid camera count");
                        if (!PhoneValidator.isValidDate(dateStr)) throw new IllegalArgumentException("Invalid date");
                        if (!PhoneValidator.isValidPrice(priceStr)) throw new IllegalArgumentException("Invalid price");

                        int id = Integer.parseInt(idStr);
                        int cameras = Integer.parseInt(camerasStr);
                        var date = PhoneValidator.parseDate(dateStr);
                        double price = PhoneValidator.parsePrice(priceStr);

                        phones.add(new Phone(id, brand, model, cameras, date, price));

                    } catch (Exception e) {
                        String errorMsg = "Error reading XML element: " + e.getMessage() + " | data: " + 
                                            String.join(";", idStr, brand, model, camerasStr, dateStr, priceStr);
                        logWriter.println("Error: " + errorMsg);
                        System.err.println("Error: " + errorMsg);
                    }
                }
            }
        } catch (Exception e) {
            try (PrintWriter logWriter = new PrintWriter(new FileWriter("errors.log", true))) {
                String errorMsg = "Error reading XML file: " + filename + " - " + e.getMessage();
                logWriter.println("Error: " + errorMsg);
                System.err.println("Error: " + errorMsg);
            } catch (IOException ioException) {
                System.err.println("Failed to write to error log: " + ioException.getMessage());
            }
        }

        return phones;
    }

    @Override
    public void writeToFile(String filename, List<Phone> data) {
        try {
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.newDocument();

            Element rootElement = doc.createElement("phones");
            doc.appendChild(rootElement);

            for (Phone p : data) {
                Element phoneElement = doc.createElement("phone");

                appendElement(doc, phoneElement, "id", String.valueOf(p.getId()));
                appendElement(doc, phoneElement, "brand", p.getBrand());
                appendElement(doc, phoneElement, "model", p.getModel());
                appendElement(doc, phoneElement, "cameraCount", String.valueOf(p.getCameraCount()));
                appendElement(doc, phoneElement, "releaseDate", PhoneValidator.formatDate(p.getReleaseDate()));
                appendElement(doc, phoneElement, "price", String.format("%.2f", p.getPrice()));

                rootElement.appendChild(phoneElement);
            }

            TransformerFactory transformerFactory = TransformerFactory.newInstance();
            Transformer transformer = transformerFactory.newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "4");
            DOMSource source = new DOMSource(doc);
            StreamResult result = new StreamResult(new File(filename));
            transformer.transform(source, result);

            System.out.println("Data successfully saved to XML file: " + filename);
        } catch (Exception e) {
            System.err.println("Error writing XML file: " + filename + " - " + e.getMessage());
        }
    }

    private String getElementValue(Element element, String tagName) {
        NodeList nodeList = element.getElementsByTagName(tagName);
        if (nodeList.getLength() > 0) {
            return nodeList.item(0).getTextContent().trim();
        }
        return "";
    }

    private void appendElement(Document doc, Element parent, String tagName, String textContent) {
        Element element = doc.createElement(tagName);
        element.setTextContent(textContent);
        parent.appendChild(element);
    }
}